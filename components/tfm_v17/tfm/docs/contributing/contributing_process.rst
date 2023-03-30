Contributing Process
====================

Contributions to the TF-M project need to follow the process below.

.. Note::

   Please contact `TF-M mailing list <mailing_list_>`_ for any question.

- It is recommended to subscribe to `TF-M mailing list <mailing_list_>`_
  via `this page <https://lists.trustedfirmware.org/mailman3/lists/tf-m.lists.trustedfirmware.org>`_.
- Refer to the `Roadmap
  <https://developer.trustedfirmware.org/w/tf_m/planning>`_ or send a mail to
  the `TF-M mailing list <mailing_list_>`_ to get the latest status and plan of
  TF-M.
- Follow :doc:`Design Proposal Guideline </contributing/tfm_design_proposal_guideline>`
  to propose your design.
- Follow guidelines below to prepare the patch:

  - Clone the TF-M code on your own machine from `TF-M git repository
    <https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git>`_.
  - Follow the :doc:`TF-M getting started </getting_started/tfm_getting_started>`,
    :doc:`Build Instructions </building/tfm_build_instruction>`
    :doc:`Coding Guide </contributing/coding_guide>` for the TF-M project.
  - Make your changes in logical chunks to help reviewers. Each commit should
    be a separate review and either work properly or be squashed after the
    review and before merging.
  - Follow :doc:`Documentation Contribution Guidelines </contributing/doc_guidelines>`
    to update documentation in ``docs`` folder if needed.
  - Test your changes and add details to the commit description.
  - The code is accepted under :doc:`Developer Certificate of Origin (DCO) </contributing/dco>`.
    Use ``git commit -s`` to add a ``Signed-off-by`` trailer at the end of the
    commit log message.
    See `git-commit <https://git-scm.com/docs/git-commit>`_ for details.
  - Add a `Change-Id <https://review.trustedfirmware.org/Documentation/user-changeid.html>`_
    to the commit message, which can be generated any way you like (e.g. from
    the SHA of the commit).
    It is suggested to clone repositories with commit-msg hook. The commit-msg
    hook attaches Change-Id automatically.
    Take `trusted-firmware-m <https://review.trustedfirmware.org/admin/repos/TF-M/trusted-firmware-m>`_
    as an example.

- Submit your patch for review.
  Refer to `Uploading Changes <https://review.trustedfirmware.org/Documentation/user-upload.html>`_
  for details of uploading patch.
- Add relevant :doc:`code owner(s) </contributing/maintainers>` for reviewing
  the patch.
- You may be asked to provide further details or make additional changes.
- You can discuss further with code owner(s) and maintainer(s) directly via
  `TF-M mailing list <mailing_list_>`_ if necessary.
- Click ``Allow-CI +1`` button on Gerrit page to run CI to validate your patch.
  Your patch shall pass CI successfully before being merged. Code owner(s) and
  maintainer(s) may ask for additional test.
- Once the change is approved by code owners, the patch will be merged by the
  maintainer.

.. _mailing_list: tf-m@lists.trustedfirmware.org

--------------

*Copyright (c) 2017-2022, Arm Limited. All rights reserved.*
