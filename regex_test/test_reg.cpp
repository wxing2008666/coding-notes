#include <regex>
#include <iostream>
#include <string>
using namespace std;

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
//替换HTML的转义符
const std::string ESCAPE_CHARACTER = "&(lt|gt|nbsp|amp|quot);";

int main() {
	std::string str_html = "<p>如图，一个正方形被分成了$$16$$个同样的小正方形．有些小正方形已被涂黑，那么最少再涂黑&nbsp;<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u>&nbsp;个小正方形可以使整个大正方形成为轴对称轴图形．</p><p style=\"text-align:center\"><span><span><span>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp;&nbsp;<span><img alt=\"\" height=\"120\" src=\"https://paper-pro-cdn.speiyou.com/imgFile/479cebac-2016-4f18-b2c3-d76effda07ba.png\" width=\"121\" /><span></span><span>\u200B</span></span><span><span>\u200B</span></span>\u200B</span></span>&nbsp; &nbsp;<span><span><span><span>\u200B</span></span></span><span>\u200B</span></span>&nbsp;</span></p><p><span><span><span><span>\u200B</span></span>\u200B</span></span><span><span><span><span><img alt=\"\" height=\"77\" src=\"https://paper-pro-cdn.speiyou.com/paper-cloud/peiyou/img/0d948ae4-bf93-4499-a6ce-3c2683846551.png\" style=\"float:right\" width=\"350\" /><span></span><span>\u200B</span></span><span>\u200B</span></span><span>\u200B</span></span><span><span><span>\u200B</span></span></span><span>\u200B</span></span></p>";
	std::cout << str_html << std::endl;
	std::cout << std::endl;
	//
	/*
	regex pattern(SPACE_REGEX);
	std::vector<std::string> results;
	const std::sregex_token_iterator end;
	for (std::sregex_token_iterator i(str_html.begin(), str_html.end(), pattern); i != end ; ++i) {
		std::cout << *i << std::endl;
		results.push_back( *i );
	}*/

	/*
	regex r1(HTML_REGEX, regex::icase);
	auto iter_begin = std::sregex_iterator(str_html.begin(), str_html.end(), r1);
	auto iter_end   = std::sregex_iterator();
	for (std::sregex_iterator i = iter_begin; i != iter_end; ++i) {
		std::smatch match = *i;
		std::string str_match = match.str();
		std::cout << str_match << std::endl;
	}*/


	//regex r1(SCRIPT_REGEX, regex::icase);
	//string new_str = regex_replace(str_html, r1, "");
	//cout << new_str << endl;
	//regex r2(STYLE_REGEX, regex::icase);
	//new_str = regex_replace(new_str, r2, "");
	//cout << new_str << endl;
	regex r3(HTML_REGEX, regex::icase);
	std::string new_str = regex_replace(str_html, r3, "");
	cout << new_str << endl;
	cout << endl;
	regex rr(ESCAPE_CHARACTER);
	new_str = regex_replace(new_str, rr, "");
	cout << new_str << endl;
	//regex r4(SPACE_REGEX, regex::icase);
	//new_str = regex_replace(new_str, r4, "");
	//cout << new_str << endl;
	return 0;
}
