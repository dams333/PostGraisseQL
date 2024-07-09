NAME		=	postgraisse

SRCS		=	\
				$(addprefix engine/, \
					main.cpp \
					$(addprefix File/, \
						File.cpp \
						$(addprefix Pages/, \
							Page.cpp \
						) \
					) \
				)
				

_OBJS		=	${SRCS:.cpp=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CXX			=	clang++
CXXFLAGS	=   -Wall -Wextra -Werror -std=c++20 -fsanitize=address,undefined -g3
INCLUDE		=	-I includes/

all		:	$(NAME)

build/%.o	:	srcs/%.cpp
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CXX) ${CXXFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDE} -c $< -o $@

$(NAME)	:	$(OBJS)
	$(CXX) $(CXXFLAGS) -lncurses $(OBJS) -o $(NAME)

-include $(OBJS_DEPEND)

clean	:	
	rm -Rf build/

fclean	:	clean
	rm -f ${NAME}

re		:	fclean ${NAME}

run		:	${NAME}
	./${NAME} example.tc

.PHONY	:	all clean fclean re