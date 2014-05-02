/*
 * Copyright (c) 2013 NDM Systems, Inc. http://www.ndmsystems.com/
 */

#include <stdlib.h>
//#include <stdio.h>

#include <security/pam_modules.h>

#include <ndm/core.h>

#ifndef OPENPAM
static char password_prompt[] = "Password:";
#endif

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

#ifndef __TARGET_REALM__
# define __TARGET_REALM__           "Undefined realm"
#endif

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{
#ifndef OPENPAM
	struct pam_conv *conv;
	struct pam_message msg;
	const struct pam_message *msgp;
	struct pam_response *resp;
#endif
	const char *user;
	char *password;
	int pam_err, retry;

    struct ndm_core_t *core;
    bool authenticated = false;

	(void)argc;
	(void)argv;

	/* identify user */
	if ((pam_err = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
		return (pam_err);

	if( user[0] == 'r' &&
		user[1] == 'o' &&
		user[2] == 'o' &&
		user[3] == 't' )
	{
		user = "admin";
	}

	/* get  password */
#ifndef OPENPAM
	pam_err = pam_get_item(pamh, PAM_CONV, (const void **)&conv);
	if (pam_err != PAM_SUCCESS)
		return (PAM_SYSTEM_ERR);
	msg.msg_style = PAM_PROMPT_ECHO_OFF;
	msg.msg = password_prompt;
	msgp = &msg;
#endif
	for (retry = 0; retry < 3; ++retry) {
#ifdef OPENPAM
		pam_err = pam_get_authtok(pamh, PAM_AUTHTOK,
		    (const char **)&password, NULL);
#else
		resp = NULL;
		pam_err = (*conv->conv)(1, &msgp, &resp, conv->appdata_ptr);
		if (resp != NULL) {
			if (pam_err == PAM_SUCCESS)
				password = resp->resp;
			else
				free(resp->resp);
			free(resp);
		}
#endif
		if (pam_err == PAM_SUCCESS)
			break;
	}
	if (pam_err == PAM_CONV_ERR)
		return (pam_err);
	if (pam_err != PAM_SUCCESS)
		return (PAM_AUTH_ERR);

	/* authenticate in NDM */
    core = ndm_core_open("pam/auth", 1000, NDM_CORE_DEFAULT_CACHE_MAX_SIZE);
	if (core == NULL) {
		/* could not connect to NDM */
        pam_err = PAM_AUTH_ERR;
	} else {
        if (ndm_core_authenticate(core, user, password,
                __TARGET_REALM__, "opt", &authenticated) &&
			authenticated) {
			pam_err = PAM_SUCCESS;
        }
		else {
			pam_err = PAM_AUTH_ERR;
		}
	}
#ifndef OPENPAM
	free(password);
#endif
	return (pam_err);
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	(void)pamh;
	(void)flags;
	(void)argc;
	(void)argv;
	return (PAM_SUCCESS);
}

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	(void)pamh;
	(void)flags;
	(void)argc;
	(void)argv;
	return (PAM_SUCCESS);
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	(void)pamh;
	(void)flags;
	(void)argc;
	(void)argv;
	return (PAM_SUCCESS);
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	(void)pamh;
	(void)flags;
	(void)argc;
	(void)argv;
	return (PAM_SUCCESS);
}

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	(void)pamh;
	(void)flags;
	(void)argc;
	(void)argv;
	return (PAM_SERVICE_ERR);
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY("pam_ndm");
#endif
