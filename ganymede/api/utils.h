#ifndef UTILS_H_
#define UTILS_H_

#define ARR_SIZE(x) (sizeof(x) / sizeof(*(x)))

#define MIN(a, b) ( ((a) < (b)) ? (a) : (b) )
#define MAX(a, b) ( ((a) < (b)) ? (b) : (a) )

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define STR_(s) #s
#define STR(s) STR_(s)

#define COMPILER_CHECK(cond) struct CONCAT(CHECK_,__COUNTER__) { int X[1-2*(!(cond))]; }

#endif /* UTILS_H_ */
