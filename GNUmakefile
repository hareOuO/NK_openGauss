#
# PostgreSQL top level makefile
#
# GNUmakefile.in
#

subdir =
top_builddir = .
include $(top_builddir)/src/Makefile.global

$(call recurse,all install,src config)

all:
	+@echo "All of openGauss successfully made. Ready to install."

docs:
	$(MAKE) -C doc all

$(call recurse,world,doc src config contrib,all)
world:
	+@echo "openGauss, contrib, and documentation successfully made. Ready to install."

# build src/ before contrib/
world-contrib-recurse: world-src-recurse

html man:
	$(MAKE) -C doc $@


ifeq ($(enable_mysql_fdw), yes)
install_mysql_fdw:
	$(MAKE) -C contrib/mysql_fdw install
else
install_mysql_fdw:
endif

ifeq ($(enable_oracle_fdw), yes)
install_oracle_fdw:
	$(MAKE) -C contrib/oracle_fdw install
else
install_oracle_fdw:
endif

ifeq ($(enable_pldebugger), yes)
install_pldebugger:
	$(MAKE) -C contrib/pldebugger install
else
install_pldebugger:
endif

#enable_privategauss supports the package feature,
#and openGauss does not hold the package feature
ifeq ($(enable_multiple_nodes), yes)
install:
	$(MAKE) install_mysql_fdw
	$(MAKE) install_oracle_fdw
	$(MAKE) -C contrib/hstore $@
	$(MAKE) -C src/distribute/kernel/extension/packages $@
	$(MAKE) -C contrib/pagehack $@
	$(MAKE) -C contrib/pg_xlogdump $@
	$(MAKE) -C contrib/gsredistribute $@
	$(MAKE) -C src/distribute/kernel/extension/dimsearch $@
	$(MAKE) -C contrib/security_plugin $@
	$(MAKE) -C src/distribute/kernel/extension/tsdb $@
	+@echo "PostgreSQL installation complete."
else
ifeq ($(enable_privategauss), yes)
install:
	$(MAKE) install_mysql_fdw
	$(MAKE) install_oracle_fdw
	$(MAKE) install_pldebugger
	$(MAKE) -C contrib/postgres_fdw $@
	$(MAKE) -C contrib/hstore $@
	$(MAKE) -C src/distribute/kernel/extension/packages $@
	$(MAKE) -C contrib/gsredistribute $@
	+@echo "openGauss installation complete."
else
install:
	$(MAKE) install_mysql_fdw
	$(MAKE) install_oracle_fdw
	$(MAKE) install_pldebugger
	$(MAKE) -C contrib/postgres_fdw $@
	$(MAKE) -C contrib/hstore $@
	$(MAKE) -C contrib/gsredistribute $@
	+@echo "openGauss installation complete."
endif
endif

install-docs:
	$(MAKE) -C doc install

$(call recurse,install-world,,doc src config contrib,install)
install-world:
	+@echo "openGauss, contrib, and documentation installation complete."

# build src/ before contrib/
install-world-contrib-recurse: install-world-src-recurse

$(call recurse,installdirs uninstall coverage,doc src config)

$(call recurse,distprep,doc src config contrib)

# clean, distclean, etc should apply to contrib too, even though
# it's not built by default
$(call recurse,clean,doc contrib src config)
clean:
# Garbage from autoconf:
	@rm -rf autom4te.cache/

# Important: distclean `src' last, otherwise Makefile.global
# will be gone too soon.
distclean maintainer-clean:
	$(MAKE) -C doc $@
	$(MAKE) -C contrib $@
	$(MAKE) -C config $@
	$(MAKE) -C src $@
	rm -f config.cache config.log config.status GNUmakefile
# Garbage from autoconf:
	@rm -rf autom4te.cache/

check: all

fastcheck: all

ifeq ($(enable_multiple_nodes), yes)
fastcheck_inplace: all

fastcheck_parallel_initdb: all

qunitcheck: all

fastcheck_single: all

redocheck: all

redochecksmall: all

redischeck: all

dfsredischeck: all

orccheckxian: all

orccheckusa: all

orcchecksmall: all

dfscheck: all

obscheck: all

obsorccheck: all

securitycheck: all

parquetchecksmall: all

check fastcheck fastcheck_inplace fastcheck_parallel_initdb qunitcheck redischeck redocheck redochecksmall orccheckxian orccheckusa orcchecksmall parquetchecksmall dfscheck obscheck obsorccheck securitycheck dfsredischeck installcheck installcheck-parallel 2pccheck fastcheck_single:
	$(MAKE) -C src/distribute/test/regress $@

#llt include all low level test
llt: reg ut

reg: all

reg:
	@echo "begin regression test..."
	$(MAKE) -C src/distribute/test reg
	@echo "end regression test"

hacheck:
	make install
	@echo "begin hacheck test..."
	$(MAKE) -C src/distribute/test/ha hacheck
	@echo "end hacheck test"

commcheck:
	make install -sj "CPPFLAGS += -DLIBCOMM_CHECK"
	@echo "begin commcheck test..."
	$(MAKE) -C src/distribute/test/commcheck commcheck
	@echo "end commcheck test"

upcheck upgradecheck:
	@echo "Attention: please make sure GAUSSHOME , prefix, PATH and LD_LIBRARY_PATH have been set right."; \
	echo "If encounting port conflicts, please change base_port in src/distribute/test/upgrade/upgradeCheck.py."; \
	sleep 5; \
	make install; \
	echo "begin upgrade test..."; \
	python src/distribute/test/upgrade/upgradeCheck.py; \
	echo "end upgrade test";

ut:
	@echo "begin unit test..."
	$(MAKE) -C src/distribute/test/ut ut
	@echo "end unit test"

$(call recurse,check-world,src/distribute/test src/pl src/interfaces/ecpg contrib,check)

$(call recurse,installcheck-world,src/distribute/test src/pl src/interfaces/ecpg contrib,installcheck)

else
ifeq ($(enable_mot), yes)
check fastcheck fastcheck_parallel_initdb fastcheck_single fastcheck_single_mot:
else
check fastcheck fastcheck_parallel_initdb fastcheck_single:
endif
	$(MAKE) -C src/test/regress $@

$(call recurse,check-world,src/test src/pl src/interfaces/ecpg contrib,check)

$(call recurse,installcheck-world,src/test src/pl src/interfaces/ecpg contrib,installcheck)

endif

$(call recurse,maintainer-check,doc src config contrib)

GNUmakefile: GNUmakefile.in $(top_builddir)/config.status
	./config.status $@


##########################################################################

distdir	= postgresql-$(VERSION)
dummy	= =install=
garbage = =*  "#"*  ."#"*  *~*  *.orig  *.rej  core  postgresql-*

dist: $(distdir).tar.gz $(distdir).tar.bz2
	rm -rf $(distdir)

$(distdir).tar: distdir
	$(TAR) chf $@ $(distdir)

.INTERMEDIATE: $(distdir).tar

distdir-location:
	@echo $(distdir)

distdir:
	rm -rf $(distdir)* $(dummy)
	for x in `cd $(top_srcdir) && find . \( -name CVS -prune \) -o \( -name .git -prune \) -o -print`; do \
	  file=`expr X$$x : 'X\./\(.*\)'`; \
	  if test -d "$(top_srcdir)/$$file" ; then \
	    mkdir "$(distdir)/$$file" && chmod 777 "$(distdir)/$$file";	\
	  else \
	    ln "$(top_srcdir)/$$file" "$(distdir)/$$file" >/dev/null 2>&1 \
	      || cp "$(top_srcdir)/$$file" "$(distdir)/$$file"; \
	  fi || exit; \
	done
	$(MAKE) -C $(distdir) distprep
	$(MAKE) -C $(distdir)/doc/src/sgml/ HISTORY INSTALL regress_README
	cp $(distdir)/doc/src/sgml/HISTORY $(distdir)/
	cp $(distdir)/doc/src/sgml/INSTALL $(distdir)/
	cp $(distdir)/doc/src/sgml/regress_README $(distdir)/src/test/regress/README
	$(MAKE) -C $(distdir) distclean
	rm -f $(distdir)/README.git

distcheck: dist
	rm -rf $(dummy)
	mkdir $(dummy)
	$(GZIP) -d -c $(distdir).tar.gz | $(TAR) xf -
	install_prefix=`cd $(dummy) && pwd`; \
	cd $(distdir) \
	&& ./configure --prefix="$$install_prefix"
	$(MAKE) -C $(distdir) -q distprep
	$(MAKE) -C $(distdir)
	$(MAKE) -C $(distdir) install
	$(MAKE) -C $(distdir) uninstall
	@echo "checking whether \`$(MAKE) uninstall' works"
	test `find $(dummy) ! -type d | wc -l` -eq 0
	$(MAKE) -C $(distdir) dist
# Room for improvement: Check here whether this distribution tarball
# is sufficiently similar to the original one.
	rm -rf $(distdir) $(dummy)
	@echo "Distribution integrity checks out."

.PHONY: dist distdir distcheck docs install-docs world check-world install-world installcheck-world