// 自带换行的颜色输出
#define BLUE(STRING) std::cout << "\033[34m" << STRING << "\033[m\n"
#define GREEN(STRING) std::cout << "\033[32m" << STRING << "\033[m\n"
#define RED(STRING) std::cout << "\033[31m" << STRING << "\033[m\n"
#define YELLOW(STRING) std::cout << "\033[33m" << STRING << "\033[m\n"

// 不带换行符的颜色输出
#define BLUE_WITHOUTENTER(STRING) std::cout << "\033[34m" << STRING << "\033[m"
#define YELLOW_WITHOUTENTER(STRING) \
  std::cout << "\033[33m" << STRING << "\033[m"
#define GREEN_WITHOUTENTER(STRING) std::cout << "\033[32m" << STRING << "\033[m"
#define RED_WITHOUTENTER(STRING) std::cout << "\033[31m" << STRING << "\033[m"