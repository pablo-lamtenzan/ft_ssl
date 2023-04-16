
#include <ssl_engine.h>
#include <ssl_utils.h>
#include <ftlibc.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

command_t* get_command(const char* cmd_name)
{
	command_t*	found = NULL;

	for (u64 i = 0 ; i < ARRLEN(commands) ; i++)
	{
		if (ft_strcmp(cmd_name, commands[i].name) == 0)
		{
			found = (command_t*)&commands[i];
			break ;
		}
	}
	if (found == NULL)
		FERROR(EFMT_CMDNOTFOUND, cmd_name);
	return found;
}

err_t	select_command(const char** cmd_input_name[], command_t** dest)
{
	err_t		st = SUCCESS;
	command_t*	found = get_command(**cmd_input_name);

	if (found == NULL)
		st = E_UNKOWNARGUMENT;
	else
	{
		*dest = found;
		++(*cmd_input_name);
	}
	return st;
}

static err_t buff_create(u64 capacity, u8** dest)
{
	err_t st = SUCCESS;

	*dest = malloc(sizeof(**dest) * capacity);
	if (*dest == NULL)
	{
		ERAISE_ERRNO("malloc");
		st = E_SYSCALL;
	}
	return st;
}

static void memcpy_strip_endl(u8* dest, u8* src, u64 n)
{
	u64 i = 0;

	for (u64 y = 0 ; y < n ; y++)
	{
		if (src[y] != '\n')
			dest[i++] = src[y];
	}
	dest[i] = '\0';
}

static err_t buff_join(u8** dest, u64 destlen, u8* other, u64 otherlen)
{
	err_t st = E_SYSCALL;

	u8* prev = *dest;
	if ((st = buff_create(destlen + otherlen + 1, dest)) == SUCCESS)
	{
		if (destlen)
			memcpy_strip_endl(*dest, prev, destlen);
		memcpy_strip_endl(*dest + destlen, other, otherlen);
	}

	free(prev);
	return st;
}

static err_t compute_from_fd(command_t* const cmd, int fd, void* const dest, result_t* const res, u8** bytes)
{
	#define BUFF_SIZE 0x1000

	err_t		st = SUCCESS;
	u64			total_msg_len = 0;
	u64			bytes_buff_count = 0;
	u8			buff1[BUFF_SIZE] = {0};
	u8			buff2[BUFF_SIZE] = {0};
	u8*			work_buff;
	ssize_t		nread1;
	ssize_t		nread2;
	ssize_t*	work_nread;
	ssize_t*	other_nread;

	cmd->init(dest);

	nread1 = read(fd, buff1, ARRLEN(buff1));
	if (nread1 < 0)
	{
		ERAISE_ERRNO("write");
		st = E_SYSCALL;
	}

	nread2 = read(fd, buff2, ARRLEN(buff2));
	if (nread2 < 0)
	{
		ERAISE_ERRNO("write");
		st = E_SYSCALL;
	}

	work_buff = buff1;
	work_nread = &nread1;
	other_nread = &nread2;

	while (true)
	{
		if (bytes)
		{
			if ((st = buff_join(bytes, bytes_buff_count, work_buff, *work_nread)) != SUCCESS)
				goto error;
			bytes_buff_count += *work_nread;
		}
		u64 i = 0;
		do
		{
			if (*work_nread <= (ssize_t)cmd->chunklen && *other_nread == 0)
			{
				total_msg_len += *work_nread;
				*res = cmd->final(dest, &work_buff[i * cmd->chunklen], *work_nread, total_msg_len);
				*work_nread = 0;
			}
			else
			{
				total_msg_len += cmd->chunklen;
				cmd->update(dest, &work_buff[i * cmd->chunklen]);
				*work_nread -= cmd->chunklen;
			}
			i++;
		} while (*work_nread > 0);

		if (*other_nread == 0)
				break ;

		if (work_buff == buff1)
		{
			work_buff = buff2;
			work_nread = &nread2;

			nread1 = read(fd, buff1, ARRLEN(buff1));
			if (nread1 < 0)
			{
				ERAISE_ERRNO("write");
				st = E_SYSCALL;
				goto error;
			}
			other_nread = &nread1;
		}
		else
		{
			work_buff = buff1;
			work_nread = &nread1;

			nread2 = read(fd, buff2, ARRLEN(buff2));
			if (nread2 < 0)
			{
				ERAISE_ERRNO("write");
				st = E_SYSCALL;
				goto error;
			}
			other_nread = &nread2;
		}
	}

error:
	return st;
}

err_t	compute_stdin(command_t* const cmd, void* const dest, result_t* const res, u8** bytes)
{
	return compute_from_fd(cmd, STDIN_FILENO, dest, res, bytes);
}

void	compute_string(command_t* const cmd, void* const dest, const char* str, result_t* const res)
{
	cmd->init(dest);
	
	u64	chunk_count = 0;
	u64	total_msg_len = 0;
	i64 string_len = (i64)ft_strlen(str);

	while (string_len > 0)
	{
		ft_memcpy((void*)chunk_buffer, (void*)&str[cmd->chunklen * chunk_count++], MIN((i64)cmd->chunklen, string_len));
		if (string_len <= (i64)cmd->chunklen)
		{
			total_msg_len += string_len;
			*res = cmd->final(dest, chunk_buffer, string_len, total_msg_len);
		}
		else
		{
			total_msg_len += cmd->chunklen;
			cmd->update(dest, chunk_buffer);
		}
		string_len -= (i64)cmd->chunklen;
	}
}

err_t	compute_file(command_t* const cmd, const char* filename, void* const dest, result_t* const res)
{
	err_t st = SUCCESS;

	const int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		if (errno == ENOENT)
		{
			FERROR(EFMT_FILENOTFOUND, cmd->name, filename);
			st = E_FILENOTFOUND;
		}
		else
		{
			ERAISE_ERRNO("open");
			st = E_SYSCALL;
		}
		goto error;		
	}

	st = compute_from_fd(cmd, fd, dest, res, NULL);
	close(fd);

error:
	return st;
}
