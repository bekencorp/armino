========================================
Protected Storage service key management
========================================

:Author: Jamie Fox
:Organization: Arm Limited
:Contact: Jamie Fox <jamie.fox@arm.com>

Background
==========
The PSA Protected Storage API requires confidentiality for external storage to
be provided by:

    **cryptographic ciphers using device-bound keys**, a tamper resistant
    enclosure, or an inaccessible deployment location, depending on the threat
    model of the deployed system.

A TBSA-M-compliant device must embed a Hardware Unique Key (HUK), which provides
the root of trust (RoT) for confidentiality in the system. It must have at least
128 bits of entropy (and a 128 bit data size), and be accessible only to Trusted
code or Trusted hardware that acts on behalf of Trusted code. [TBSA-M]_

Design description
==================
Each time the system boots, PS will request that the Crypto service uses a key
derivation function (KDF) to derive a storage key from the HUK, by referring to
the builtin key handle for the HUK. The storage key could be kept in on-chip
volatile memory private to the Crypto partition, or it could remain inside a
secure element. Either way it will not be returned to PS.

For each call to the PSA Protected Storage APIs, PS will make requests to the
Crypto service to perform AEAD encryption and/or decryption operations using the
storage key (providing a fresh nonce for each encryption).

At no point will PS access the key material itself, only referring to the HUK
and storage key by their handles in the Crypto service.

Key derivation
==============
PS will make key derivation requests to the Crypto service with calls to the
PSA Crypto APIs. In order to derive the storage key, the following calls are
required:

.. code-block:: c

    status = psa_key_derivation_setup(&op, PSA_ALG_HKDF(PSA_ALG_SHA_256));

    /* Set up a key derivation operation with HUK  */
    status = psa_key_derivation_input_key(&op, PSA_KEY_DERIVATION_INPUT_SECRET,
                                          TFM_BUILTIN_KEY_ID_HUK);

    /* Supply the PS key label as an input to the key derivation */
    status = psa_key_derivation_input_bytes(&op, PSA_KEY_DERIVATION_INPUT_INFO,
                                            key_label,
                                            key_label_len);

    /* Create the storage key from the key derivation operation */
    status = psa_key_derivation_output_key(&attributes, &op, &ps_key);

.. note::
    ``TFM_BUILTIN_KEY_ID_HUK`` is a static key ID that is used to identify the
    HUK. It has an arbitrary value defined in ``tfm_crypto_defs.h``

    ``ps_key`` is a PSA Crypto key handle to a volatile key, set by the
    derivation operation. After the call to ``psa_key_derivation_output_key``,
    it can be used to refer the storage key.

    ``key_label`` can be any string that is independent of the input key
    material and different to the label used in any other derivation from the
    same input key. It prevents two different contexts from deriving the same
    output key from the same input key.

The key derivation function used by the crypto service to derive the storage key
will be HKDF, with SHA-256 as the underlying hash function. HKDF is suitable
because:

- It is simple and efficient, requiring only two HMAC operations when the length
  of the output key material is less than or equal to the hash length (as is the
  case here).
- The trade-off is that HKDF is only suitable when the input key material has at
  least as much entropy as required for the output key material. But this is the
  case here, as the HUK has 128 bits of entropy, the same as required by PS.
- HKDF is standardised in RFC 5869 [RFC5869]_ and its security has been formally
  analysed. [HKDF]_
- It is supported by the TF-M Crypto service.

The choice of underlying hash function is fairly straightforward: it needs to be
a modern standardised algorithm, considered to be secure and supported by TF-M
Crypto. This narrows it down to just the SHA-2 family. Of the hash functions in
the family, SHA-256 is the simplest and provides more than enough output length.

Keeping the storage key private to PS
-------------------------------------

The Crypto service derives a platform key from the HUK, using the partition ID
as the input to that derivation, and that platform key is used for the key
derivation by PS. This happens transparently, and to PS is indistinguishable
from deriving from the HUK except that other partitions cannot derive the
storage key even if they know the derivation parameters.

Key use
=======
To encrypt and decrypt data, PS will call the PSA Crypto AEAD APIs in the same
way as the current implementation, but ``ps_key`` will refer to the storage key,
rather than the imported HUK. For each encryption operation, the following call
is made (and analogously for decryption):

.. code-block:: c

    psa_aead_encrypt(ps_key, PS_CRYPTO_ALG,
                     crypto->ref.iv, PS_IV_LEN_BYTES,
                     add, add_len,
                     in, in_len,
                     out, out_size, out_len);

References
==========
.. [TBSA-M] Arm Platform Security Architecture Trusted Base System Architecture
   for Armv6-M, Armv7-M and Armv8-M, version 1.0
.. [HKDF] Hugo Krawczyk. 2010. Cryptographic extraction and key derivation: the
   HKDF scheme. In Proceedings of the 30th annual conference on Advances in
   cryptology (CRYPTO'10)
.. [RFC5869] IETF RFC 5869: HMAC-based Extract-and-Expand Key Derivation
   Function (HKDF)

--------------

*Copyright (c) 2019-2022, Arm Limited. All rights reserved.*
