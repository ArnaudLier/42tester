/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 09:49:43 by alier             #+#    #+#             */
/*   Updated: 2024/10/30 10:41:59 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "libft.h"

void	test_lstnew(void)
{
	int		i;
	t_list	*lst;

	printf("Testing Project Libft: ft_lstnew.\n");
	i = 42;
	lst = ft_lstnew(&i);
	if (lst->content != &i)
	{
		fprintf(stderr, "lst->content was not set to given pointer\n");
		exit(EXIT_FAILURE);
	}
	if (lst->next != NULL)
	{
		fprintf(stderr, "lst->next was not NULL\n");
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
}

void	test_lstadd_front(void)
{
	int		i;
	int		i2;
	t_list	*lst;
	t_list	*temp;
	t_list	*lst2;

	printf("Testing Project Libft: ft_lstadd_front.\n");
	i = 42;
	i2 = 21;
	lst = ft_lstnew(&i);
	temp = lst;
	lst2 = ft_lstnew(&i2);
	ft_lstadd_front(&lst, lst2);
	if (lst != lst2)
	{
		fprintf(stderr, "lst was not updated with new element\n");
		exit(EXIT_FAILURE);
	}
	if (lst->content != &i2)
	{
		fprintf(stderr, "lst->content is incorrect\n");
		exit(EXIT_FAILURE);
	}
	if (lst->next == NULL)
	{
		fprintf(stderr, "lst->next was NULL\n");
		exit(EXIT_FAILURE);
	}
	if (lst->next != temp)
	{
		fprintf(stderr, "lst->next was not the correct element\n");
		exit(EXIT_FAILURE);
	}
	if (lst->next->content != &i)
	{
		fprintf(stderr, "lst->next->content is incorrect\n");
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
}

void	test_lstsize(void)
{
	t_list	*lst;
	t_list	*lst2;
	t_list	*lst3;
	int		size;

	printf("Testing Project Libft: ft_lstsize.\n");
	lst = ft_lstnew(NULL);
	lst2 = ft_lstnew(NULL);
	lst3 = ft_lstnew(NULL);
	ft_lstadd_front(&lst, lst2);
	ft_lstadd_front(&lst, lst3);
	size = ft_lstsize(lst);
	if (size != 3)
	{
		fprintf(stderr, "got %d instead of 3", size);
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
}

void	test_lstlast(void)
{
	t_list	*lst;
	t_list	*lst2;
	t_list	*lst3;

	printf("Testing Project Libft: ft_lstlast.\n");
	lst = ft_lstnew(NULL);
	lst2 = ft_lstnew(NULL);
	lst3 = ft_lstnew(NULL);
	ft_lstadd_front(&lst, lst2);
	ft_lstadd_front(&lst, lst3);
	if (ft_lstlast(lst) != lst2->next)
	{
		fprintf(stderr, "last was not the last element of the list");
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
	if (ft_lstlast(NULL) != NULL)
	{
		fprintf(stderr, "does not handle NULL input");
		exit(EXIT_FAILURE);
	}
}

void	test_lstadd_back(void)
{
	t_list	*lst;
	t_list	*lst2;

	printf("Testing Project Libft: ft_lstadd_back.\n");
	lst = ft_lstnew(NULL);
	lst2 = ft_lstnew(NULL);
	ft_lstadd_back(&lst, lst2);
	if (lst->next != lst2)
	{
		fprintf(stderr, "element was not added at the back");
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
	lst = NULL;
	lst2 = ft_lstnew(NULL);
	ft_lstadd_back(&lst, lst2);
	if (lst != lst2)
	{
		fprintf(stderr, "didn't modify passed pointer to new initial element");
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
}

void	test_lstdelone(void)
{
	t_list	*lst;
	t_list	*lst2;

	printf("Testing Project Libft: ft_lstdelone.\n");
	lst = ft_lstnew(NULL);
	lst2 = ft_lstnew(NULL);
	ft_lstadd_back(&lst, lst2);
	ft_lstdelone(lst, NULL);
	ft_lstclear(&lst2, NULL);
}

void	test_lstclear(void)
{
	t_list	*lst;
	t_list	*lst2;
	t_list	*lst3;

	printf("Testing Project Libft: ft_lstclear.\n");
	lst = ft_lstnew(NULL);
	lst2 = ft_lstnew(NULL);
	lst3 = ft_lstnew(NULL);
	ft_lstadd_front(&lst, lst2);
	ft_lstadd_front(&lst, lst3);
	ft_lstclear(&lst, free);
	if (lst != NULL)
	{
		fprintf(stderr, RED "pointer was not set to NULL with non-NULL destructor\n" RESET);
		exit(EXIT_FAILURE);
	}
}

void	lstiter_inc(void *content)
{
	int	*i;

	i = content;
	(*i)++;
}

void	test_lstiter(void)
{
	t_list	*lst;
	t_list	*lst2;
	int		i;
	int		j;

	printf("Testing Project Libft: ft_lstiter.\n");
	i = 42;
	j = 21;
	lst = ft_lstnew(&i);
	lst2 = ft_lstnew(&j);
	ft_lstadd_back(&lst, lst2);
	ft_lstiter(lst, lstiter_inc);
	if (*(int *) lst->content != 43)
	{
		fprintf(stderr, "function was not applied on element\n");
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	if (*(int *) lst->next->content != 22)
	{
		fprintf(stderr, "function was not applied on element\n");
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&lst, NULL);
}

void	*lstmap_inc(void *content)
{
	int	*i;

	i = malloc(sizeof(int));
	*i = *((int *) content) + 1;
	return (i);
}

void	lstmap_dealloc(void *content)
{
	free(content);
}

void	test_lstmap(void)
{
	t_list	*lst;
	t_list	*lst2;
	t_list	*new_lst;
	int		i;
	int		j;

	printf("Testing Project Libft: ft_lstmap.\n");
	i = 42;
	j = 21;
	lst = ft_lstnew(&i);
	lst2 = ft_lstnew(&j);
	ft_lstadd_back(&lst, lst2);
	new_lst = ft_lstmap(lst, lstmap_inc, lstmap_dealloc);
	if (*(int *) new_lst->content != 43)
	{
		fprintf(stderr, "function was not applied on element (%d)",
			*(int *)new_lst->content);
		ft_lstclear(&new_lst, lstmap_dealloc);
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	if (*(int *) new_lst->next->content != 22)
	{
		fprintf(stderr, "function was not applied on element (%d)",
			*(int *)new_lst->next->content);
		ft_lstclear(&new_lst, lstmap_dealloc);
		ft_lstclear(&lst, NULL);
		exit(EXIT_FAILURE);
	}
	ft_lstclear(&new_lst, lstmap_dealloc);
	ft_lstclear(&lst, NULL);
}
