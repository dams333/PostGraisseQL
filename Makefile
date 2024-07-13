NAME		=	postgraisse

SRCS		=	\
				$(addprefix engine/, \
					main.cpp \
					$(addprefix File/, \
						File.cpp \
						FilesManager.cpp \
						$(addprefix Page/, \
							Page.cpp \
							$(addprefix Tuple/, \
								IntTupleElementHandler.cpp \
								StringTupleElementHandler.cpp \
							) \
						) \
					) \
					$(addprefix Database/, \
						Database.cpp \
						Table.cpp \
					) \
				)
				

_OBJS		=	${SRCS:.cpp=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CXX			=	clang++
CXXFLAGS	=   -Wall -Wextra -Werror -std=c++17 -fsanitize=address,undefined -g3
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

pageexplorer:
	@mkdir -p bin
	make -C tools/PageExplorer

tools: pageexplorer
	@mkdir -p bin
	@cp tools/PageExplorer/pageexp bin/

.PHONY	:	all clean fclean re