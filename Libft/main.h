/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:13:55 by alier             #+#    #+#             */
/*   Updated: 2024/10/16 11:18:30 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef int	t_chrclasser(int);

typedef struct s_strlcpy_test_case
{
	size_t			size;
	size_t			expected_return;
	const char		*src;
	const char		*expected_dst;
}				t_sctt;

typedef struct s_strlcat_test_case
{
	size_t			size;
	size_t			expected_return;
	const char		*src;
	const char		*dst;
	const char		*expected_dst;
}				t_scatt;

typedef struct s_strncmp_test_case
{
	size_t			size;
	const char		*s1;
	const char		*s2;
}				t_scntt;

typedef struct s_memchr_test_case
{
	const char		*s;
	const char		c;
	const size_t	n;
}				t_mchtt;

typedef struct s_memcmp_test_case
{
	size_t		size;
	const char	*s1;
	const char	*s2;
}				t_mctt;

typedef struct s_strnstr_test_case
{
	const char		*big;
	const char		*little;
	size_t			size;
}				t_snstt;

typedef struct s_substr_test_case
{
	unsigned int	start;
	size_t			len;
	const char		*src;
	const char		*result;
}				t_sstt;

typedef struct s_strjoin_test_case
{
	const char	*s1;
	const char	*s2;
	const char	*expected_result;
}				t_sjtt;

typedef struct s_strtrim_test_case
{
	const char	*s1;
	const char	*set;
	const char	*expected_result;
}				t_sttc;

typedef struct s_split_test_case
{
	const char	*s;
	char		c;
	char		*expected[4];
}				t_spltc;

typedef struct s_itoa_test_case
{
	int			n;
	const char	*expected_result;
}				t_iatc;

typedef struct s_strmapi_test_case
{
	char const	*src;
	char		(*f)(unsigned int, char);
	const char	*expected_result;
}				t_smatc;

typedef struct s_striteri_test_case
{
	char const	*src;
	void		(*f)(unsigned int, char *);
	const char	*expected_result;
}				t_siitc;

typedef struct s_putnbr_test_case
{
	int		input;
	char	*expected;
}				t_pntt;
