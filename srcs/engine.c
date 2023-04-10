
#include <ssl_engine.h>
#include <ssl_utils.h>

#include <string.h> // TODO: remove
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

err_t	select_command(const char** cmd_input_name[], command_t** dest)
{
	const char* const cmd_names[] = {
		CMD_NAME_MD5,
		CMD_NAME_SHA256,
		CMD_NAME_SHA512
	};

	err_t				st = SUCCESS;
	command_t*			found = NULL;

	for (u64 i = 0 ; i < ARRLEN(cmd_names) ; i++)
	{
		if (strcmp(**cmd_input_name, cmd_names[i]) == 0)
		{
			found = (command_t*)&commands[i];
			break ;
		}
	}
	if (found == NULL)
	{
		st = E_UNKOWNARGUMENT;
		FERROR(EFMT_CMDNOTFOUND, **cmd_input_name);
	}
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

static err_t buff_join(u8** dest, u64 destlen, u8* other, u64 otherlen)
{
	err_t st = E_SYSCALL;

	u8* prev = *dest;
	if ((st = buff_create(destlen + otherlen, dest)) == SUCCESS)
	{
		if (destlen)
			memcpy(*dest, prev, destlen);
		memcpy(*dest + destlen, other, otherlen);
	}

	free(prev);
	return st;
}

static err_t compute_from_fd(command_t* const cmd, int fd, void* const dest, result_t* const res, u8** bytes)
{
	err_t	st = SUCCESS;
	ssize_t	nread;
	u64		total_msg_len = 0;
	u64		bytes_buff_count = 0;

	cmd->init(dest);

	nread = read(fd, chunk_buffer, cmd->chunklen);
	if (nread < 0)
	{
		ERAISE_ERRNO("write");
		st = E_SYSCALL;
	}
	///TODO: In this case hash an empty string
	else if (nread == 0)
		st = E_UNAVALAIBLE;
	else
	{
		do
		{
			if (bytes)
			{
				if ((st = buff_join(bytes, bytes_buff_count, chunk_buffer, cmd->chunklen)) != SUCCESS)
					goto error;
				bytes_buff_count += cmd->chunklen;
			}

			if (nread <= (ssize_t)cmd->chunklen)
			{
				total_msg_len += nread;
				*res = cmd->final(dest, chunk_buffer, (u64)nread, total_msg_len);
			}
			else
			{
				total_msg_len += cmd->chunklen;
				cmd->update(dest, chunk_buffer);
			}
		} while ((nread = read(fd, chunk_buffer, cmd->chunklen)) > 0);
		if (nread < 0)
		{
			ERAISE_ERRNO("write");
			st = E_SYSCALL;
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
	i64 string_len = (i64)strlen(str);

	while (string_len > 0)
	{
		memcpy((void*)chunk_buffer, (void*)&str[cmd->chunklen * chunk_count++], MIN((i64)cmd->chunklen, string_len));
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