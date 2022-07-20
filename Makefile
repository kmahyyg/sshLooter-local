CFLAGS += -Werror -Wall

all: clean pam_sshaudit.so mkpamexec

mkdeb: clean pam_sshaudit.so mvdeb

pam_sshaudit.so: pam_sshaudit.c
	gcc $(CFLAGS) -fPIC -shared -Xlinker -x -o $@ $<

clean:
	rm -f pam_sshaudit.so
	sudo rm -f /usr/lib/x86_64-linux-gnu/security/pam_sshaudit.so
	sudo rm -f /usr/lib64/security/pam_sshaudit.so

mvdeb:
	sudo cp pam_sshaudit.so /usr/lib/x86_64-linux-gnu/security/pam_sshaudit.so

mvcentos7:
	sudo cp pam_sshaudit.so /usr/lib64/security/pam_sshaudit.so

mkpamexec:
	GOOS=linux GOARCH=amd64 CGO_ENABLED=0 garble -tiny -seed=random -literals build -o sshaudit -ldflags='-s -w' -trimpath sshaudit.go

