/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oagrram <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 10:50:58 by oagrram           #+#    #+#             */
/*   Updated: 2019/12/02 10:51:20 by oagrram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

t_flist  *new_node(void)
{
    t_flist *node;
    node =  (t_flist*) malloc(sizeof(t_flist));
    return(node);
}

void     getpermition(struct stat fileStat, t_flist **node)
{
    struct passwd *user;
    struct group *group;

    user = getpwuid(fileStat.st_uid);
    group = getgrgid(user->pw_gid);
    (*node)->permision[0] = ( (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
    (*node)->permision[1] = ( (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
    //printf("res == %o S_IWUSR == %o mode == %o\n",(fileStat.st_mode & S_IWUSR),S_IWUSR,fileStat.st_mode);
    (*node)->permision[2] = ( (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
    (*node)->permision[3] = ( (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
    (*node)->permision[4] = ( (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
    (*node)->permision[5] = ( (fileStat.st_mode & S_IXGRP) ? 'x' : '-'); 
    (*node)->permision[6] = ( (fileStat.st_mode & S_IROTH) ? 'r' : '-');
    (*node)->permision[7] = ( (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
    //printf("res == %o S_IWOTH == %o mode == %o\n",(fileStat.st_mode & S_IWOTH),S_IWOTH,fileStat.st_mode);
    (*node)->permision[8] = ( (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
    //printf("res == %o S_IXOTH == %o mode == %o\n",(fileStat.st_mode & S_IXOTH),S_IXOTH,fileStat.st_mode);
    (*node)->permision[9] = (' ');
    (*node)->permision[9] = ('\0');
    (*node)->nlink = (fileStat.st_nlink);
    (*node)->user =(user->pw_name);
    (*node)->groupe =(group->gr_name);
    (*node)->size = (fileStat.st_size);
}

char   getfiletype(mode_t    mode)
{
    if (S_ISREG(mode))
        return ('-');
    else if (S_ISDIR(mode))
        return ('d');
    else if (S_ISBLK(mode))
       return ('b');
    else if (S_ISCHR(mode))
        return ('c');
    #ifdef S_ISFIFO
    else if (S_ISFIFO(mode))
        return ('p');
    #endif
    #ifdef S_ISLNK
    else if (S_ISLNK(mode))
        return ('l');
    #endif
    #ifdef S_ISSOCK
    else if (S_ISSOCK(mode))
        return ('s');
    #endif
    #ifdef S_ISDOOR
    else if (S_ISDOOR(mode))
        return ('D');
    #endif
    return('?');
}

int stockage(struct stat fileStat , t_flist **node)
{
    (*node)->mtime = fileStat.st_mtime;
    (*node)->mtime = fileStat.st_mtime;
    (*node)->time= ft_strsub(ctime(&fileStat.st_mtime),4,12);
    (*node)->type= getfiletype(fileStat.st_mode);
    getpermition(fileStat,&(*node));
    return (0);
}

int     ft_flist(char *name)
{
    data       system;

    lstat(name, &(system.fileStat));
    system.head = NULL;
    if (S_ISDIR(system.fileStat.st_mode))
    {
        if (!(system.dir = opendir(name)))
        {
            perror("ft_ls :");
            return (0);
        }
        while((system.sd = readdir(system.dir)) != NULL)
        {
            system.node =  (t_flist*)ft_memalloc(sizeof(t_flist));
            system.path = ft_strjoin(name, "/");
            //printf("path == %s\n",system.path);
            system.path = ft_strjoin(name, (system.sd)->d_name);
            system.node->name = ft_strdup((system.sd)->d_name);
            lstat(system.path, &(system.fileStat)); 
            stockage(system.fileStat,&system.node);
           // system.node->next =  (t_flist*) malloc(sizeof(t_flist));
           // system.node = system.node->next;
            //system.node->next = NULL;
            system.node->mtime = system.fileStat.st_mtime;
            sort_by_time(&system.node, &system.head);
        }
         printlist(&system.head);
        freelist(&system.head);
        closedir(system.dir);
    }
    else
    {
        stockage(system.fileStat,&system.node);
        system.node->next=NULL;
        printfile(&system.node,name);
        free(system.node);
    }
    return (0);
}

int     main(int argc,char **argv)
{
    argc = 2;
    printf("\n-\t-\t-\t-\t-\t-\t-\t-");
    printf("\n\n");
   ft_flist(argv[1]);
    return (0);
}
