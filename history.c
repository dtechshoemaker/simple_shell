#include "shell.h"

/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(info_t *info)
{
	char *dir = _getenv(info, "HIME=");
	if (!dir)
		return (NULL);

	size_t buf_size = _strlen(dir) + _strlen(HIST_FILE) + 2;
	char *buf = malloc(buf_size);
	if (!buf)
		return (NULL);

	buf[0] = '\0';
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *info)
{
	char *filename = get_history_file(info);
	if (!filename)
		return (-1);

	size_t fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);

	list_t *node = info->history;
	while (node)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
		node = node->next;
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: history count on success, 0 otherwise
 */
int read_history(info_t *info)
{	char *filename = get_history_file(info);
	if (!filename)
		return (0);
	ssize_t fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	struct stat st;
	if (fstat(fd, &st) != 0)
	{
		close(fd);
		return (0);
	}
	off_t fsize = st.st_size;
	if (fsize < 2)
	{
		close(fd);
		return (0);
	}
	char *buf = malloc(fsize + 1);
	if (!buf)
	{
		close(fd);
		return (0);
	}
	ssize_t rdlen = read(fd, buf, fsize);
	close(fd);
	if (rdlen <= 0)
	{
		free(buf);
		return (0);
	}
	buf[fsize] = '\0';
	int linecount = 0;
	int last = 0;
	for (int i = 0; i < fsize; i++)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	}
	if (last != fsize)
build_history_list(info, buf + last, linecount++);
free(buf);
info->histcount = linecount;
while (info->histcount-- >= HIST_MAX)
delete_node_at_index(&(info->history), 0);
renumber_history(info);
return (info->histcount);
}

/**
 * build_history_list - adds an entry to the history linked list
 * @info: parameter struct
 * @buf: buffer
 * @linecount: history linecount, histcount
 *
 * Return: 0 on success
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: parameter struct
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}

	info->histcount = i;
	return (i);
}
