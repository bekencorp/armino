# "Common" Makefile for Sphinx documentation
#
# (included from en/Makefile & zh_CN/Makefile
#
# NOTE: This makefile runs with cwd=either en or zh_CN subfolder, so this
# (docs/) directory is '..' relative to it.

# ************ IMPORTANT *****************
#
# ReadTheDocs DOES NOT USE THIS MAKEFILE,
# so any behaviour additions must be
# done via Sphinx Config not here
#
# ****************************************

# You can set these variables from the command line.
SPHINXOPTS    =
# note: this is changed from sphinx-build so it depends on default python interpreter, not on /bin/sphinx-build
# (which will be the most recently installed version of sphinx and may not match)
#SPHINXBUILD   = python -m sphinx
SPHINXBUILD   = sphinx-build
PROCESSES     = -jauto
#PROCESSES     =
PAPER         =
BUILDDIR      = _build
DOCS_VERSION := latest

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/$(DOCS_VERSION)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) -w sphinx-warning-log.txt .
# the i18n builder cannot share the environment and doctrees with the others
I18NSPHINXOPTS  = $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .

START_TIME := $(shell date +%s)

.PHONY: help clean html dirhtml singlehtml pickle json htmlhelp qthelp devhelp epub latex latexpdf text man changes linkcheck doctest gettext dependencies version-specific-includes check_python_packages

help:
	@echo "Please use \`make <target>\' where <target> is one of"
	@echo "  html       to make standalone HTML files"
	@echo "  dirhtml    to make HTML files named index.html in directories"
	@echo "  singlehtml to make a single large HTML file"
	@echo "  pickle     to make pickle files"
	@echo "  json       to make JSON files"
	@echo "  htmlhelp   to make HTML files and a HTML help project"
	@echo "  qthelp     to make HTML files and a qthelp project"
	@echo "  devhelp    to make HTML files and a Devhelp project"
	@echo "  epub       to make an epub"
	@echo "  latex      to make LaTeX files, you can set PAPER=a4 or PAPER=letter"
	@echo "  latexpdf   to make LaTeX files and run them through pdflatex"
	@echo "  latexpdfja to make LaTeX files and run them through platex/dvipdfmx"
	@echo "  text       to make text files"
	@echo "  man        to make manual pages"
	@echo "  texinfo    to make Texinfo files"
	@echo "  info       to make Texinfo files and run them through makeinfo"
	@echo "  gettext    to make PO message catalogs"
	@echo "  changes    to make an overview of all changed/added/deprecated items"
	@echo "  xml        to make Docutils-native XML files"
	@echo "  pseudoxml  to make pseudoxml-XML files for display purposes"
	@echo "  linkcheck  to check all external links for integrity"
	@echo "  doctest    to run all doctests embedded in the documentation (if enabled) "

clean:
	rm -rf $(BUILDDIR)/*

# Notify users when some of the required python packages are not installed.
# Note: This is intended to help developers who generate the documentation on their local machine. Read The Docs uses
# the requirements.txt file directly and calls sphinx also directly without the use of the makefile!
check_python_packages:
	#$(ARMINO_PATH)/tools/check_python_dependencies.py -r $(ARMINO_PATH)/docs/requirements.txt

html: | check_python_packages
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html $(PROCESSES)
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."

arminodocs: | check_python_packages
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/${DOCS_VERSION} $(PROCESSES)
	@echo "check_python_packages"
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."
	@current_time=`date +%s`; \
	time_interval=`expr $${current_time} - $(START_TIME)`; \
	runtime=`date -u -d @$${time_interval} +%Hh:%Mm:%Ss`; \
	echo ${YELLOW}"######## runtime: $${runtime} ########"${DEFAULT}
	../check_doc_warnings.sh

dirhtml: | check_python_packages
	$(SPHINXBUILD) -b dirhtml $(ALLSPHINXOPTS) $(BUILDDIR)/dirhtml $(PROCESSES)
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/dirhtml."

singlehtml: | check_python_packages
	$(SPHINXBUILD) -b singlehtml $(ALLSPHINXOPTS) $(BUILDDIR)/singlehtml $(PROCESSES)
	@echo
	@echo "Build finished. The HTML page is in $(BUILDDIR)/singlehtml."

pickle: | check_python_packages
	$(SPHINXBUILD) -b pickle $(ALLSPHINXOPTS) $(BUILDDIR)/pickle $(PROCESSES)
	@echo
	@echo "Build finished; now you can process the pickle files."

json: | check_python_packages
	$(SPHINXBUILD) -b json $(ALLSPHINXOPTS) $(BUILDDIR)/json $(PROCESSES)
	@echo
	@echo "Build finished; now you can process the JSON files."

htmlhelp: | check_python_packages
	$(SPHINXBUILD) -b htmlhelp $(ALLSPHINXOPTS) $(BUILDDIR)/htmlhelp $(PROCESSES)
	@echo
	@echo "Build finished; now you can run HTML Help Workshop with the" \
	      ".hhp project file in $(BUILDDIR)/htmlhelp."

qthelp: | check_python_packages
	$(SPHINXBUILD) -b qthelp $(ALLSPHINXOPTS) $(BUILDDIR)/qthelp $(PROCESSES)
	@echo
	@echo "Build finished; now you can run "qcollectiongenerator" with the" \
	      ".qhcp project file in $(BUILDDIR)/qthelp, like this:"
	@echo "# qcollectiongenerator $(BUILDDIR)/qthelp/ReadtheDocsTemplate.qhcp"
	@echo "To view the help file:"
	@echo "# assistant -collectionFile $(BUILDDIR)/qthelp/ReadtheDocsTemplate.qhc"

devhelp: | check_python_packages
	$(SPHINXBUILD) -b devhelp $(ALLSPHINXOPTS) $(BUILDDIR)/devhelp $(PROCESSES)
	@echo
	@echo "Build finished."
	@echo "To view the help file:"
	@echo "# mkdir -p $$HOME/.local/share/devhelp/ReadtheDocsTemplate"
	@echo "# ln -s $(BUILDDIR)/devhelp $$HOME/.local/share/devhelp/ReadtheDocsTemplate"
	@echo "# devhelp"

epub: | check_python_packages
	$(SPHINXBUILD) -b epub $(ALLSPHINXOPTS) $(BUILDDIR)/epub $(PROCESSES)
	@echo
	@echo "Build finished. The epub file is in $(BUILDDIR)/epub."

latex: | check_python_packages
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex $(PROCESSES)
	@echo
	@echo "Build finished; the LaTeX files are in $(BUILDDIR)/latex."
	@echo "Run \`make' in that directory to run these through (pdf)latex" \
	      "(use \`make latexpdf' here to do that automatically)."

latexpdf: | check_python_packages
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex $(PROCESSES)
	@echo "Running LaTeX files through pdflatex..."
	$(MAKE) -C $(BUILDDIR)/latex all-pdf
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex."

latexpdfja: | check_python_packages
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex $(PROCESSES)
	@echo "Running LaTeX files through platex and dvipdfmx..."
	$(MAKE) -C $(BUILDDIR)/latex all-pdf-ja
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex."

text: | check_python_packages
	$(SPHINXBUILD) -b text $(ALLSPHINXOPTS) $(BUILDDIR)/text $(PROCESSES)
	@echo
	@echo "Build finished. The text files are in $(BUILDDIR)/text."

man: | check_python_packages
	$(SPHINXBUILD) -b man $(ALLSPHINXOPTS) $(BUILDDIR)/man $(PROCESSES)
	@echo
	@echo "Build finished. The manual pages are in $(BUILDDIR)/man."

texinfo: | check_python_packages
	$(SPHINXBUILD) -b texinfo $(ALLSPHINXOPTS) $(BUILDDIR)/texinfo $(PROCESSES)
	@echo
	@echo "Build finished. The Texinfo files are in $(BUILDDIR)/texinfo."
	@echo "Run \`make' in that directory to run these through makeinfo" \
	      "(use \`make info' here to do that automatically)."

info: | check_python_packages
	$(SPHINXBUILD) -b texinfo $(ALLSPHINXOPTS) $(BUILDDIR)/texinfo $(PROCESSES)
	@echo "Running Texinfo files through makeinfo..."
	make -C $(BUILDDIR)/texinfo info -j32
	@echo "makeinfo finished; the Info files are in $(BUILDDIR)/texinfo."

gettext: | check_python_packages
	$(SPHINXBUILD) -b gettext $(I18NSPHINXOPTS) $(BUILDDIR)/locale $(PROCESSES)
	@echo
	@echo "Build finished. The message catalogs are in $(BUILDDIR)/locale."

changes: | check_python_packages
	$(SPHINXBUILD) -b changes $(ALLSPHINXOPTS) $(BUILDDIR)/changes $(PROCESSES)
	@echo
	@echo "The overview file is in $(BUILDDIR)/changes."

linkcheck: | check_python_packages
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck $(PROCESSES)
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/output.txt."

gh-linkcheck: | check_python_packages
	@echo "Checking for hardcoded GitHub links"
	@echo "TODO ---------------->"
	@echo "No hardcoded links found"

doctest: | check_python_packages
	$(SPHINXBUILD) -b doctest $(ALLSPHINXOPTS) $(BUILDDIR)/doctest $(PROCESSES)
	@echo "Testing of doctests in the sources finished, look at the " \
	      "results in $(BUILDDIR)/doctest/output.txt."

xml: | check_python_packages
	$(SPHINXBUILD) -b xml $(ALLSPHINXOPTS) $(BUILDDIR)/xml $(PROCESSES)
	@echo
	@echo "Build finished. The XML files are in $(BUILDDIR)/xml."

pseudoxml: | check_python_packages
	$(SPHINXBUILD) -b pseudoxml $(ALLSPHINXOPTS) $(BUILDDIR)/pseudoxml $(PROCESSES)
	@echo
	@echo "Build finished. The pseudo-XML files are in $(BUILDDIR)/pseudoxml."
