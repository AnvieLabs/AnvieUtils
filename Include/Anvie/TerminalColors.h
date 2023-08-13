/**
 * @file TerminalColors.h
 * @date Wed, 5th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2023 Siddharth Mishra
 * @copyright Copyright 2023 Anvie Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @brief Defines linux terminal colors for printing colored text
 * usage COLOR_RED "text" COLOR_RESET -> This will print "text" in red color.
 * */

#ifndef ANIVE_UTILS_TERMINAL_COLORS_H
#define ANIVE_UTILS_TERMINAL_COLORS_H

#define COLOR_BLACK "\e[0;30m"
#define COLOR_RED "\e[0;31m"
#define COLOR_GREEN "\e[0;32m"
#define COLOR_YELLOW "\e[0;33m"
#define COLOR_BLUE "\e[0;34m"
#define COLOR_PURPLE "\e[0;35m"
#define COLOR_CYAN "\e[0;36m"
#define COLOR_WHITE "\e[0;37m"
#define COLOR_BOLD_BLACK "\e[1;30m"
#define COLOR_BOLD_RED "\e[1;31m"
#define COLOR_BOLD_GREEN "\e[1;32m"
#define COLOR_BOLD_YELLOW "\e[1;33m"
#define COLOR_BOLD_BLUE "\e[1;34m"
#define COLOR_BOLD_PURPLE "\e[1;35m"
#define COLOR_BOLD_CYAN "\e[1;36m"
#define COLOR_BOLD_WHITE "\e[1;37m"
#define COLOR_UNDERLINE_BLACK "\e[4;30m"
#define COLOR_UNDERLINE_RED "\e[4;31m"
#define COLOR_UNDERLINE_GREEN "\e[4;32m"
#define COLOR_UNDERLINE_YELLOW "\e[4;33m"
#define COLOR_UNDERLINE_BLUE "\e[4;34m"
#define COLOR_UNDERLINE_PURPLE "\e[4;35m"
#define COLOR_UNDERLINE_CYAN "\e[4;36m"
#define COLOR_UNDERLINE_WHITE "\e[4;37m"
#define COLOR_BACKGROUND_BLACK "\e[40m"
#define COLOR_BACKGROUND_RED "\e[41m"
#define COLOR_BACKGROUND_GREEN "\e[42m"
#define COLOR_BACKGROUND_YELLOW "\e[43m"
#define COLOR_BACKGROUND_BLUE "\e[44m"
#define COLOR_BACKGROUND_PURPLE "\e[45m"
#define COLOR_BACKGROUND_CYAN "\e[46m"
#define COLOR_BACKGROUND_WHITE "\e[47m"
#define COLOR_HIGH_INTENSITY_BLACK "\e[0;90m"
#define COLOR_HIGH_INTENSITY_RED "\e[0;91m"
#define COLOR_HIGH_INTENSITY_GREEN "\e[0;92m"
#define COLOR_HIGH_INTENSITY_YELLOW "\e[0;93m"
#define COLOR_HIGH_INTENSITY_BLUE "\e[0;94m"
#define COLOR_HIGH_INTENSITY_PURPLE "\e[0;95m"
#define COLOR_HIGH_INTENSITY_CYAN "\e[0;96m"
#define COLOR_HIGH_INTENSITY_WHITE "\e[0;97m"
#define COLOR_BOLD_HIGH_INTENSITY_BLACK "\e[1;90m"
#define COLOR_BOLD_HIGH_INTENSITY_RED "\e[1;91m"
#define COLOR_BOLD_HIGH_INTENSITY_GREEN "\e[1;92m"
#define COLOR_BOLD_HIGH_INTENSITY_YELLOW "\e[1;93m"
#define COLOR_BOLD_HIGH_INTENSITY_BLUE "\e[1;94m"
#define COLOR_BOLD_HIGH_INTENSITY_PURPLE "\e[1;95m"
#define COLOR_BOLD_HIGH_INTENSITY_CYAN "\e[1;96m"
#define COLOR_BOLD_HIGH_INTENSITY_WHITE "\e[1;97m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_BLACK "\e[0;100m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_RED "\e[0;101m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_GREEN "\e[0;102m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_YELLOW "\e[0;103m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_BLUE "\e[0;104m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_PURPLE "\e[0;105m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_CYAN "\e[0;106m"
#define COLOR_HIGH_INTENSITY_BACKGROUND_WHITE "\e[0;107m"
#define COLOR_RESET "\e[0m"

#endif // ANIVE_UTILS_TERMINAL_COLORS_H
