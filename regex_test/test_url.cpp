#include <sys/types.h>
#include <sys/time.h>
#include <regex.h>
#include <string.h>
#include <iostream>
#include <string>

//图片提取正则表达式
const std::string IMG_REGEX = "<img[^>]+src=\"([^\">]+)\"[^>]+\\/>";
//定义script的正则表达式，去除js可以防止注入
const std::string SCRIPT_REGEX = "<script[^>]*?>[\\s\\S]*?<\\/script>";
//定义style的正则表达式，去除style样式，防止css代码过多时只截取到css样式代码
const std::string STYLE_REGEX = "<style[^>]*?>[\\s\\S]*?<\\/style>";
//定义HTML标签的正则表达式，去除标签，只提取文字内容
const std::string HTML_REGEX = "<[^>]+>";
//定义空格,回车,换行符,制表符
const std::string SPACE_REGEX = "\\s*|\t|\r|\n";
 
bool init(regex_t &reg)
{
	int status;
	//支持正则表达式扩展标签
	int cflags = REG_EXTENDED;
	// 过滤规则:以http://开头或以https://开头
	const char * pattern = "(http://[a-zA-Z0-9./?%&_=-]+)|(https://[a-zA-Z0-9./?%&_=-]+)|(width[a-zA-Z0-9\"]+)";
	//编译正则表达式
	if((status = regcomp(&reg, pattern, cflags)) != 0)
	{
		std::cerr << "pattern compile error" << std::endl;
		char err_buff[1024];
		regerror(status, &reg, err_buff, 1024);
		std::cout << "error message:" << err_buff << std::endl;
		return false;
	}	
	return true;
}
 
bool destroy(regex_t &reg)
{
	regfree(&reg);
}
 
bool url_filter(regex_t &reg, std::string &input, std::string &output) 
{
	int status;	
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	// regex函数必须匹配'\0'结尾的字符串
	input.append(1,'\0');
	char *st = new char[input.length()];
	input.copy(st, input.length(), 0);
	//循环匹配多次
	while (st && (status = regexec(&reg, st, nmatch, pmatch, REG_NOTEOL)) != REG_NOMATCH)
	{
		int num = pmatch[0].rm_eo - pmatch[0].rm_so;
		//删除匹配到的字符串
		input.erase(pmatch[0].rm_so, num);
		input.copy(st, input.length(), 0);
	}
	// 删除掉我们添加的'\0'
	output = input.erase(input.length() - 1, 1);
	delete [] st;
	return true;
}
 
// test 
int main(int argc, char *argv[])
{
	std::string str_html = "<p>如图，一个正方形被分成了$$16$$个同样的小正方形．有些小正方形已被涂黑，那么最少再涂黑&nbsp;<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u>&nbsp;个小正方形可以使整个大正方形成为轴对称轴图形．</p><p style=\"text-align:center\"><span><span><span>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp;&nbsp;<span><img alt=\"\" height=\"120\" src=\"https://paper-pro-cdn.speiyou.com/imgFile/479cebac-2016-4f18-b2c3-d76effda07ba.png\" width=\"121\" /><span></span><span>\u200B</span></span><span><span>\u200B</span></span>\u200B</span></span>&nbsp; &nbsp;<span><span><span><span>\u200B</span></span></span><span>\u200B</span></span>&nbsp;</span></p><p><span><span><span><span>\u200B</span></span>\u200B</span></span><span><span><span><span><img alt=\"\" height=\"77\" src=\"https://paper-pro-cdn.speiyou.com/paper-cloud/peiyou/img/0d948ae4-bf93-4499-a6ce-3c2683846551.png\" style=\"float:right\" width=\"350\" /><span></span><span>\u200B</span></span><span>\u200B</span></span><span>\u200B</span></span><span><span><span>\u200B</span></span></span><span>\u200B</span></span></p>";
	std::string input, output;
	input = "width\"143\" 此电摩VID_20160531_194403.mp4 一出，http://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=0&rsv_idx=1&tn=baidu&wd=%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F%20%E5%AD%97%E7%AC%A6%20%26&rsv_pq=cd1686ec0020b7eb&rsv_t=21a17dKwt642CgDFL8JxM%2Ft1ywlP4OECwVMil5Ij%2FbCWJdOc6UXRr%2BMaDr4&rqlang=cn&rsv_enter=1&rsv_sug3=27&rsv_sug1=20&rsv_sug7=101&rsv_sug2=0&inputT=7511&rsv_sug4=7997 谁还要汽车  http://www.arxql.zx58.cn/wanghao/1.html 玩微信的朋友可以加我微信gxys6666[em]e400905[/em]号点这里[em]e400389[/em]二维码页面,post:_wv 1 srctype touch apptype iphone loginuin 120340009 plateform mobileqq url http%253A%252F%252Fqm.qq.com%252Fcgi-bin%252Fqm%252Fqr%253Fk%253D3t-ZOf2mUGLPAheKhi2l_c5KmisysqWH src_uin 120340009 src_scene 311 cli_scene getDetailzuzu气垫BB，只涂了半边脸，提亮肤色，特别保湿，遮盖力超级好，不挂粉，一整天都不脱妆  快来围观我的精彩微视频！ https://xiaoying.tv/v/e3qd9/2/?fromApp XiaoYing toApp qzone（通过#小影#创作）";
	std::cout << "input:" << std::endl;
	std::cout << str_html << std::endl << std::endl;
	//
	regex_t reg;
	if(!init(reg))
	{
		std::cerr << "init reg error" << std::endl;
		return 1;
	}
	//
	int cflags = REG_EXTENDED;
	regcomp(&reg, IMG_REGEX.c_str(), cflags);
	//
	url_filter(reg, str_html, output);
	//
	regcomp(&reg, SCRIPT_REGEX.c_str(), cflags);
	url_filter(reg, str_html, output);
	regcomp(&reg, STYLE_REGEX.c_str(), cflags);
	url_filter(reg, str_html, output);
	regcomp(&reg, HTML_REGEX.c_str(), cflags);
	url_filter(reg, str_html, output);
	//regcomp(&reg, SPACE_REGEX.c_str(), cflags);
	//url_filter(reg, str_html, output);
	//
	destroy(reg);
	//
	std::cout << "output:" << std::endl;
	std::cout << output << std::endl << std::endl;
	return 0;
}
