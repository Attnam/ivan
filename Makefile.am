SUBDIRS = FeLib Main Script Graphics

EXTRA_DIST = IVAN.dsw FeLib.dsp Main.dsp ivandj.mak ivanmgw.mak LICENSING .customs.emacs

install-data-local:
	-./mkinstalldirs -p $(localstatedir) $(bindir) $(datadir)/ivan/Script/ $(datadir)/ivan/Graphics/ $(localstatedir)/ivan/Bones/
	-touch $(localstatedir)/ivan/ivan-highscore.scores
	-chmod 666 $(localstatedir)/ivan/ivan-highscore.scores
	-chmod -R u=rwx $(localstatedir)/ivan/Bones/
	-chmod -R g=rwx $(localstatedir)/ivan/Bones/
	-chmod -R o=rwx $(localstatedir)/ivan/Bones/
	-chmod 2111 $(bindir)/ivan
