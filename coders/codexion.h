/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:01 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:15 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

/* Standard library includes */
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

/* Project headers */
# include "structs.h"
# include "prototypes.h"
# define USAGE_MSG "Usage: %s <coder_count> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<required_compiles> <dongle_cooldown> <scheduler>\n"
#endif