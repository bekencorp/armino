##############################
Mailbox NS Agent Design Update
##############################

:Organization: Arm Limited
:Contact: tf-m@lists.trustedfirmware.org

**********
Background
**********
The SPE component that maintains the non-secure clients' request is called
'NS Agent' in TF-M. Besides the Trustzone-based isolation mechanism, there is
one other isolation mechanism that implements individual PEs in physically
isolated cores respectively. NSPE and SPE transfer non-secure client requests
via inter-processor communication based on mailboxes. The component that
handles inter-processor communication messages is called ``mailbox NS Agent``.

.. note::
  There may be hardware components and software solutions containing 'mailbox'
  in their names. The concept ``mailbox`` in this document represent the
  mechanism described above, which is not referring to the external concepts.

When the first version ``mailbox NS Agent`` was introduced, the generic FFM
interrupt handling was not ready. Hence a customized solution
``Multiple Core`` is implemented. This customized implementation:

- Perform customized operations on SPM internal data in a deferred interrupt
  handler.
- Process mailbox operations as special cases in SPM common logic.

These behaviours couple SPM tightly with mailbox logic, which bring issues for
maintenance. To address the issue, an updated design shall:

- Make SPM manage other components in a unified way (For example, it is
  simpler for SPM if all non-SPM components under the IPC model act as
  ``processes``.)
- Can use FF-M compliant interrupt mechanism and APIs.

Following the above guidelines makes the ``mailbox NS agent`` work like a
``partition``. The agent has an endless loop and waits for signals, calls FFM
API based on the parsing result on the communication messages. But there are
still issues after looking closer to the requirements of the agent:

- SPM treats FFM Client API caller's ID as the client ID. While the mailbox NS
  agent may represent multiple non-secure clients. Hence it needs to tell
  SPM which non-secure client it is representing, and the default FFM Client
  API does not have such capability.
- FFM Client API blocks caller before the call is replied; while the
  mailbox NS Agent needs to respond to the non-secure interrupts in time.
  Blocking while waiting for a reply may cause the non-secure communication
  message not to be handled in time.

Extra design items need to be added to address the issues.

*************
Design Update
*************
The below figure shows the overall design to cover various component types.
NS Agents are the implementation-defined components that provide FF-M compliant
Client API to the non-secure clients. Hence from the view of the non-secure
clients, the FF-M client API behaviour follows the FF-M definition. And NS
Agent needs customization in SPM since it has extra requirements compared to
a generic secure partition.

.. figure:: media/mailbox_ns_agent1.*
    :align: center
    :name: fig-mailbox1
    :width: 70%

    Component types and the callable API set

.. note::
  3 non-SPM component types here: FFM-compliant Secure Partition
  (aka ``partition``), Trustzone-based NS Agent (aka ``Trustzone NS agent``)
  and mailbox-based NS Agent (aka ``mailbox NS agent``).
  ``Trustzone NS agent`` is mentioned here for the comparison purpose. The
  implementation details for this NS agent type is not introduced here.

To make the programming model close to the FFM compliance, the
``mailbox NS agent`` is designed as:

- Working like a standard Secure Partition under the IPC model, has one
  single thread, can call FFM standard API.
- Having a manifest file to describe the attributes and resources and a
  positive valued ``Partition ID`` in the manifest.

Services rely on the ``client_id`` to apply policy-checking, hence SPM
needs to know which ``client_id`` the mailbox NS Agent is representing when
mailbox API is calling Client API. The standard API treats the caller as the
client of the service, which means that an extended API is needed to support
identifying the non-secure client that is represented. SPM sets the
non-secure ``client_id`` into the message right at the moment the message is
going to be sent. Before this point, SPM performs the call based on the
agent's ID.

These ``Extended APIs`` are non-blocking, unlike the standard FF-M Client
APIs. This can improve the communication efficiency between NS clients and
mailbox NS agents. With this mechanism, extra signals and APIs for message
acknowledges are also required.

.. note::
  A standard Secure Partition gets errors when calling the ``Extended API``.

The secure mapped memory for mailbox communication is one of the agent's
assets. As the agent has the capability to forward and represent non-secure
clients, it is the agent's duty to identify the non-secure clients it is
representing.

Updated Programming Items
=========================
These Client APIs are the expansion based on the standard Client APIs:

- ``agent_psa_connect`` is extended from ``psa_connect``.
- ``agent_psa_call`` is extended from ``psa_call``.

And to cooperate with the changed behaviour of these APIs, extra defined
signals and types are also involved.

.. note::
  Namespace ``agent`` is involved for NS Agent callable API; namespace ``tfm``
  is involved for TF-M specific concepts. Even though ``agent`` is TF-M
  specific at the current stage, it is proposed to be a common concept for
  general FF-M compliant implementations, hence assigning ``agent`` for
  proposed API and data structures.

Agent-specific Client API
=========================
``agent_psa_connect`` is the API added to support agent forwarding NS
requests.

.. code-block:: c

  psa_handle_t agent_psa_connect(uint32_t sid, uint32_t version,
                                 int32_t ns_client_id, void *client_data);

One extra parameter ``ns_client_id`` added to tell SPM which NS client the
agent is representing when API gets called. It is recorded in the handle
association data in SPM and requires to be a negative value; ZERO or positive
values are invalid non-secure client IDs, SPM does not use these invalid IDs
in the message. Instead, it puts the agent's ID into the messaging in this
case. This mechanism can provide chances for the agents calling APIs for their
own service accessing and API works asynchronously.

As mentioned, the standard FFM Client service accessing API are blocked until
the IPC message gets replied to. This API returns immediately without waiting
for acknowledgement. A ``psa_handle_t`` is allocated and returned if no error
occurred because a unique value is needed to help the agent manage the
non-secure clients and the requests. The subsequent ``agent_psa_call``
or ``agent_psa_close`` with this allocated but not acknowledged handle gets
an ``In progress`` status code.

.. code-block:: c

  typedef struct {
      psa_invec     in_vecs[PSA_MAX_IOVEC];
      psa_outvec    out_vecs[PSA_MAX_IOVEC];
  } client_vectors_t;

  typedef struct {
      int32_t ns_client_id;
      void    *client_data;
  } client_param_t;

  psa_status_t agent_psa_call(psa_handle_t handle, int32_t type,
                              client_vectors_t *vecs, client_param_t *params);

Compared to the standard ``psa_call``, this API:

- Is asynchronous.
- Squashes the ``psa_invec_t`` and ``psa_outvec_t`` into a new composited
  parameter ``vecs`` to make the ABI APCS compliant.
- one extra parameter ``params`` for ``agent_psa_call`` stands for the
  auxiliary data added. This member is ignored for connection-based services
  because ``agent_psa_connect`` already assigned one hence this member is for
  stateless services only. And the same, ZERO or positive values are ignored
  during the ``agent_psa_call`` lifecycle.

.. note::
  For stateless services, a temporary ``psa_handle_t`` is allocated and
  returned. This handle is freed after the agent read the acknowledgement by
  ``psa_get``. This is also introduced in the subsequent introduction on
  `Agent-specific signal`_.

Agent-specific signal
=====================
To cooperate with the agent-specific API, one extra acknowledgement signal is
defined:

.. code-block:: c

  #define PSA_MSG_ACK            (0x00000004u)

This signal can be sent to agent type component only. An agent can call
``psa_get`` with this signal to get one acknowledged message. This signal is
cleared when all queued messages for the agent have been retrieved using
``psa_get``. SPM assembles the information into agent provided message object.
For the stateless handle, the internal handle object is freed after this
``psa_get`` call. The agent can know what kind of message is acknowledged by
the ``type`` member in the ``psa_msg_t``, and the ``client_data`` passed in is
put in member ``rhandle``. If no 'PSA_MSG_ACK' signals pending, calling
``psa_get`` gets an state representing ``not ready`` to the caller. This
state is to be defined.

Code Example
============

.. code-block:: c

  /*
   * The actual implementation can change this __customized_t freely, or
   * discard this type and apply some in-house mechanism - the example
   * here is to introduce how an agent works only.
   */
  struct __customized_t {
      int32_t      type;
      int32_t      client_id;
      psa_handle_t handle;
      psa_handle_t status;
  };

  void mailbox_main(void)
  {
      psa_signal_t   signals;
      psa_status_t   status;
      psa_msg_t      msg;
      client_param_t client_param;
      struct __customized_t ns_msg;

      while (1) {
          signals = psa_wait(ALL, BLOCK);

          if (signals & MAILBOX_INTERRUPT_SIGNAL) {
              /* NS memory check needs to be performed. */
              __customized_platform_get_mail(&ns_msg);

              /*
               * MACRO 'SID', 'VER', 'NSID' and 'VECTORS' represents necessary
               * information extraction from 'ns_msg', put MACRO names here
               * and leave the details to the implementation.
               */
              if (ns_msg.type == PSA_IPC_CONNECT) {
                  ns_msg.handle = agent_psa_connect(SID(ns_msg), VER(ns_msg),
                                                    NSID(ns_msg), &ns_msg);
              } else if (ns_msg.type == PSA_IPC_CLOSE) {
                  psa_close(ns_msg.handle);
              } else {
                  /* Other types as call type and let API check errors. */
                  client_param.ns_client_id = ns_msg.client_id;
                  client_param.client_data  = &ns_msg;

                  ns_msg.status = agent_psa_call(ns_msg.handle,
                                                 ns_msg.type,
                                                 VECTORS(ns_msg),
                                                 &client_param);
                  /* Handle the stateless service case. */
                  if (ns_msg.handle == NULL &&
                      ns_msg.status != PSA_ERROR_IN_PROGRESS) {
                      ns_msg.handle = (psa_handle_t)ns_msg.status;
                  }
              }
          } else if (signals & PSA_MSG_ACK) {
              /* The handle is freed for stateless service after 'psa_get'. */
              status        = psa_get(PSA_MSG_ACK, &msg);
              ms_msg        = msg.rhandle;
              ns_msg.status = status;
              __customized_platform__send_mail(&ns_msg);
          }
      }
  }

.. note::
  ``__customized*`` API are implementation-specific APIs to be implemented by
  the mailbox Agent developer.

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
