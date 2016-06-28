#include "log.h"

int main()
{
	loginit(INFO, "/log", "system");

	log(ERROR, "当前日期: %s", "20160628");
	log(ERROR, "世界新闻");
	return 0;
}
