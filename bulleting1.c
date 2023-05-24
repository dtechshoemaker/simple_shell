#include "shell.h"

/**
 * shell_history - displays the history list, one command per line, preceded
 * by line numbers starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int shell_history(info_t *info)
{
    print_list(info->history);
    return 0;
}

/**
 * shell_unset_alias - unsets an alias by removing it from the alias list
 * @info: parameter struct
 * @str: the string representing the alias
 *
 * Return: Always 0 on success, 1 on error
 */
int shell_unset_alias(info_t *info, char *str)
{
    char *p, c;
    int ret;

    p = _strchr(str, '=');
    if (!p)
        return 1;
    c = *p;
    *p = '\0';
    ret = delete_node_at_index(&(info->alias),
                               get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
    *p = c;
    return ret;
}


/**
 * shell_set_alias - sets an alias to a string
 * @info: parameter struct
 * @str: the string representing the alias
 *
 * Return: Always 0 on success, 1 on error
 */
int shell_set_alias(info_t *info, char *str)
{
    char *p;

    p = _strchr(str, '=');
    if (!p)
        return 1;
    if (!*++p)
        return shell_unset_alias(info, str);

    shell_unset_alias(info, str);
    return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * shell_print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int shell_print_alias(list_t *node)
{
    char *p = NULL, *a = NULL;

    if (node)
    {
        p = _strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            _putchar(*a);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return 0;
    }
    return 1;
}

/**
 * shell_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int shell_alias(info_t *info)
{
    int i = 0;
    char *p = NULL;
    list_t *node = NULL;

    if (info->argc == 1)
    {
        node = info->alias;
        while (node)
        {
            shell_print_alias(node);
            node = node->next;
        }
        return 0;
    }
    for (i = 1; info->argv[i]; i++)
    {
        p = _strchr(info->argv[i], '=');
        if (p)
            shell_set_alias(info, info->argv[i]);
        else
            shell_print_alias(node_starts_with(info->alias, info->argv[i], '='));
    }

    return 0;
}
