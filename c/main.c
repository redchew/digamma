/* The REPL & friends (prelude loading, &c.)
 * which are not really required for when we build
 * things like libraries. Also, this means 
 * I can have multiple REPLs which simply load the "core"
 * library.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vesta.h"
extern const char *typenames[];
extern int quit_note;
int
main(int ac, char **al, char **el)
{
	/* tmp is used for building *command-line* */
	SExp *ret = nil, *tmp = nil;
	int iter = 0, rc = 0;
	char choice = 0, buf[256] = {0};
	struct stat st;
	const char *VER = "2009.3", *REL = "6.6-theta";
	const char *paths[] = {
		"./prelude.ss",
		"~/.digamma/prelude.ss",
		/*"/usr/share/digamma/prelude.ss",
		"/usr/local/share/diamma/prelude.ss",*/
		0
	};
	Symbol *tl_env = nil;
	
	if(!gc_init())
	{
		printf("[-] cannot initialize gc system...\n");
		return 1;
	}
	tl_env = init_env(); /* initialize constants */
	if(tl_env == nil)
	{
		printf("wtf\n");
		return 1;
	}
	/* register desktop spec-level I/O & Unix-interaction functions */
	register_procedure(f_princ,"display",0,tl_env);
	register_procedure(f_write,"write",0,tl_env);
	register_procedure(newline,"newline",0,tl_env);
	register_procedure(f_load,"load",0,tl_env);
	register_procedure(format,"format",0,tl_env);
	register_procedure(macro_expand,"macro-expand",0,tl_env);
	register_procedure(syntax_expand,"syntax-expand",0,tl_env);
	register_procedure(f_quit,"quit",0,tl_env);
	register_procedure(interrogate,"interrogate",0,tl_env);
	register_procedure(f_system,"system",0,tl_env);
	register_procedure(f_pwd,"pwd",0,tl_env);
	register_procedure(f_cd,"cd",0,tl_env);
	register_procedure(f_cd,"chdir",0,tl_env);
	register_procedure(f_ls,"ls",0,tl_env);
	register_procedure(f_open,"open",0,tl_env);
	register_procedure(f_read,"read",0,tl_env);
	register_procedure(f_close,"close",0,tl_env);
	register_procedure(f_read_char,"read-char",0,tl_env);
	register_procedure(f_read_string,"read-string",0,tl_env);
	register_procedure(f_write_char,"write-char",0,tl_env);
	register_procedure(f_dial,"dial",0,tl_env);
	register_procedure(f_announce,"announce",0,tl_env);
	register_procedure(f_listen,"listen",0,tl_env);
	register_procedure(f_accept,"accept",0,tl_env);
	register_procedure(f_hangup,"hangup",0,tl_env);
	register_procedure(f_remote_openp,"remote-port-open?",0,tl_env);
	register_procedure(f_gethostbyname,"gethostbyname",0,tl_env);
	register_procedure(f_gethostbyaddr,"gethostbyaddr",0,tl_env);
	register_procedure(f_rete,"rete",0,tl_env);
	register_procedure(f_read_buf,"read-buffer",0,tl_env);
	register_procedure(f_write_buf,"write-buffer",0,tl_env);
	register_procedure(f_peekchar,"peek-char",0,tl_env);
	register_procedure(f_port_filename,"port-filename",0,tl_env);
	register_procedure(f_port_mode,"port-mode",0,tl_env);
	register_procedure(f_port_bind,"port-remote-port",0,tl_env);
	register_procedure(f_port_proto,"port-protocol",0,tl_env);
	register_procedure(f_port_state,"port-state",0,tl_env);
	register_procedure(f_port_type,"port-type",0,tl_env);
	register_procedure(f_random,"random",0,tl_env);
	register_procedure(f_seed_random,"seed-random",0,tl_env);
	/* unix specific; should really be in a lib called 'unix'.
	 * take the pythonic approach & make "os.unix"?
	 * maybe just (use "nix") ^_^
	 */
	register_procedure(f_getuid,"getuid",0,tl_env);
	register_procedure(f_geteuid,"geteuid",0,tl_env);
	register_procedure(f_getgid,"getgid",0,tl_env);
	register_procedure(f_getegid,"getegid",0,tl_env);
	register_procedure(f_setsid,"set*id",0,tl_env);
	register_procedure(f_sysopen,"sysopen",0,tl_env);
	register_procedure(f_sysclose,"sysclose",0,tl_env);
	register_procedure(f_sysread,"sysread",0,tl_env);
	register_procedure(f_syswrite,"syswrite",0,tl_env);
	register_procedure(f_syspipe,"syspipe",0,tl_env);
	register_procedure(f_fork,"sysfork",0,tl_env);
	register_procedure(f_waitpid,"syswait",0,tl_env);
	register_procedure(f_execve,"sysexec",0,tl_env);
	register_procedure(f_popen,"popen",0,tl_env);
	register_procedure(f_pclose,"pclose",0,tl_env);
	register_procedure(f_vfork,"sysvfork",0,tl_env);
	register_procedure(f_kill,"syskill",0,tl_env);
	register_procedure(f_stat,"sysstat",0,tl_env);
	register_procedure(f_ssockopt,"sys*sockopt",0,tl_env);
	register_procedure(f_gettimeofday,"sysgettimeofday",0,tl_env);
	register_procedure(f_time,"systime",0,tl_env);
	register_procedure(f_chown,"syschown",0,tl_env);
	register_procedure(f_chmod,"syschmod",0,tl_env);
	register_procedure(f_chroot,"syschroot",0,tl_env);
	register_procedure(f_getenv,"sys-getenv",0,tl_env);
	register_procedure(f_setenv,"sys-setenv",0,tl_env);
	register_procedure(f_sysfcntl,"sysfcntl",0,tl_env);
	register_procedure(f_sysfcntlconst,"sysfcntl-const",0,tl_env);
	register_procedure(f_syssleep,"syssleep",0,tl_env);
	register_procedure(f_sysusleep,"sysusleep",0,tl_env);
	register_procedure(f_sysnanosleep,"sysnanosleep",0,tl_env);
	register_procedure(f_sysselect,"sysselect",0,tl_env);
	/*
	ret = f_load(cons(makestring("init.ss"),tl_env->snil),(void *)tl_env);
	if(ret != tl_env->strue)
	{
		if(ret->type == ERROR)
			printf("Error: could not load prelude from init.ss: %s\n",ret->object.error.message);
		else
			printf("Warning: could not load prelude from init.ss\n");
	}*/
	while(paths[iter] != 0)
	{
		if(paths[iter][0] == '~') // search home directory, but expand ~ 
		{
			snprintf(buf,256,"%s%s",getenv("HOME"),&paths[iter][1]);
			//printf("buf == %s\n",buf);
			rc = stat(buf,&st);
			if(!rc)
			{
				ret = f_load(cons(makestring(buf),tl_env->snil),(void *)tl_env);
				if(ret != tl_env->strue)
				{
					if(ret->type == ERROR)
						printf("Error: could not load prelude from %s: %s\n",buf,ret->object.error.message);
					else
						printf("Warning: could not load prelude from %s; continuing down paths\n",buf);
				}
				else
					choice = 1;
			}
		}
		else
		{
			rc = stat(paths[iter],&st);
			if(!rc)
			{
				ret = f_load(cons(makestring(paths[iter]),tl_env->snil),(void *)tl_env);
				if(ret != tl_env->strue)
				{
					if(ret->type == ERROR)
						printf("Error: could not load prelude from %s: %s\n",paths[iter],ret->object.error.message);
					else
						printf("Warning: could not load prelude from %s; continuing down paths\n",buf);
				}
				else
					choice = 1;
			}
		}
		if(choice)
			break;
		iter++;
	}
	if(ac != 1)
	{
		/* process command line arguments... */
		if(ac > 2)
		{
			/* build command line */
			tmp = makevector(ac - 2, nil);
			for(iter = 2;iter < ac;iter++)
				tmp->object.vec[iter - 2] = makestring(al[iter]);
			add_env(tl_env,"*command-line*",tmp);
		}
		else
			add_env(tl_env,"*command-line*",tl_env->snil);
		ret = f_load(cons(makestring(al[1]),tl_env->snil),(void *)tl_env);
		if(ret != tl_env->strue)
			if(ret->type == ERROR)
				printf("Warning: could not execute file %s: %s\n",al[1],ret->object.error.message);
		goto exit_main;
	}
	/* (define *command-line* '()) when we're not being run in script mode;
	 * useful for just testing...
	 */
#ifndef STEALTH
printf("\t()\n\
\t  ()\n\
\t()  ()\n\
Digamma/Vesta: %s/%s\n",VER, REL);
#endif /* STEALTH */
	add_env(tl_env,"*command-line*",tl_env->snil);
	/*printf("Rigors:\n");
	printf("tl_env->snil == nil? %s\n", (tl_env->snil == nil ? "true" : "false"));
	printf("tl_env->snil->type == %d\n",tl_env->snil->type);*/
	while(1)
	{
		printf("; ");
		ret = llread(stdin);
		if(feof(stdin))
		{
			clearerr(stdin);
			printf("\nExit F (y/n)? ");
			choice = fgetc(stdin);
			if(choice == 'y' || choice == 'Y')
				break;
			printf("resuming interaction\n");
			continue;
		}
		if(ret->type == ATOM && !strcmp(ret->object.str,"quit!"))
			break;
		if(ret == tl_env->fake_rsqr)
		{
			printf("extra trailing vector bracket\n");
			continue;
		}
		if(ret == tl_env->fake_rpar)
		{
			printf("extra trailing closing parenthesis\n");
			continue;
		}
		if(ret == tl_env->fake_rcur)
		{
			printf("extra trailing closing curly bracket\n");
			continue;
		}
		if(ret->type == ATOM)
		{
			if(tl_env == nil)
				printf("WARNING: tl_env == nil\n");
			ret = symlookup(ret->object.str,tl_env);
			if(ret == nil)
			{
				printf("[-] undefined binding\n");
				continue;
			}
		}
		else if(ret->type == PAIR)
			ret = lleval(ret,tl_env);
		if(quit_note)
			break;
		if(ret->type != ERROR && ret != tl_env->svoid)
		{
			printf("_ : %s = ",typenames[ret->type]);
			if(ret->type == PAIR)
				printf("'");
			llprinc(ret,stdout,1);
			printf("\n");
			if(ret->type != ATOM)
				tl_env = add_env(tl_env,"_",ret);
		}
		else if(ret != tl_env->svoid)
			printf("[-] uncaught error in F interaction: %s (s: %d,l: %d)\n",ret->object.error.message,ret->object.error.source, ret->object.error.level);
	}
exit_main:
	clean_env();
	return 0;
}