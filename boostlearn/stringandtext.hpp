#ifndef LEE_STRING
#define LEE_STRING

#include <iostream>

using namespace std;
using namespace boost;

#include <boost/lexical_cast.hpp>

//验证转换字符串的合法性
template<typename T>
bool num_valid(const char* str)
{
	try
	{
		lexical_cast<T>(str);
		return true;
	}
	catch (bad_lexical_cast&)
	{
		return false;
	}
}

template<typename T>
struct outable
{
	friend std::ostream& operator<<(std::ostream& os, const T& x)
	{
		os << typeid(T).name();
		return os;
	}
};

namespace stringandtext
{
	class demo_class :outable < demo_class > {};
};


void lexical_cast_usage()
{
	int x = lexical_cast<int>("100");
	long y = lexical_cast<long>("2000");
	float pai = lexical_cast<float>("3.14159e5");
	double e = lexical_cast<double>("2.171828");

	cout << x << y << pai << e << endl;

	string str = lexical_cast<string>(456);
	cout << str << endl;
	cout << lexical_cast<string>(0.618) << endl;
	cout << lexical_cast<string>(0x10) << endl;
	cout << lexical_cast<bool>("1") << endl;

	//bad_lexical_cast
	try
	{
		cout << lexical_cast<int>("0x10");
		cout << lexical_cast<double>("Hello World");
		cout << lexical_cast<long>("1000L");
		cout << lexical_cast<bool>("false");
	}
	catch(bad_lexical_cast& e)
	{
		cout << "error:" << e.what() << endl;
	}

	assert(num_valid<double>("3.14"));
	assert(!num_valid<int>("3.14"));
	assert(num_valid<int>("65535"));

	cout << lexical_cast<string>(stringandtext::demo_class()) << endl;
}

#include <boost/format.hpp>

void format_usage()
{
	cout << format("%s:%d+%d=%d\n") % "sum" % 1 % 2 % (1 + 2);

	format fmt("(%1% + %2%) * %2% = %3%\n");
	fmt % 2 % 5;
	fmt % ((2+5)*5);
	cout << fmt.str();

	format fmt1("%5d\n%-8.3f\n%10s\n%05X\n");
	//format fmt1("%|5d|\n%|-8.3f|\n%|10s|\n%|05X|\n");
	cout << fmt1 % 62 % 2.236 % "123465789" % 48;

	format fmt2("%1% %2% %3% %2% %1% \n");
	cout << fmt2 % 1 % 2 % 3;

	fmt2.bind_arg(2, 10);
	cout << fmt2 % 1 % 3;

	fmt2.clear();
	cout << fmt2 % group(showbase, oct, 111) % 333;

	fmt2.clear_binds();

	fmt2.modify_item(1,group(hex,right,showbase,setw(8),setfill('*')));
	cout << fmt2 % 49 % 20 % 100;
}

#include <boost/algorithm/string.hpp>

void string_algo_usage()
{
	string str("readme.txt");

	if (ends_with(str,"txt"))
	{
		cout << to_upper_copy(str) + " UPPER" << endl;
		assert(ends_with(str,"txt"));
	}

	replace_first(str, "readme", "followme");
	cout << str << endl;

	vector<char> v(str.begin(), str.end());
	vector<char> v2 = to_upper_copy(erase_first_copy(v, "txt"));
	for each (char c in v2)
	{
		cout << c;
	}

	cout << endl;

	string str1("I Don't Know.\n");
	cout << to_upper_copy(str1);
	cout << str1;
	to_lower(str1);
	cout << str1;

	string str2("Power Bomb");

	assert(iends_with(str2,"bomb"));
	assert(!ends_with(str2,"bomb"));

	assert(starts_with(str2,"Pow"));
	assert(contains(str2,"er"));

	string str3 = to_lower_copy(str2);
	assert(iequals(str2,str3));

	string str4("power suit");
	assert(ilexicographical_compare(str2,str4));

	assert(all(str3.substr(0,5),is_lower()));

	string str5("Samus"), str6("samus");

	assert(!is_equal()(str5,str6));
	assert(is_less()(str5,str6));
}

void trim_usage()
{
	format fmt("|%s|\n");

	string str = "  samus aran   ";
	cout << fmt % trim_copy(str);
	cout << fmt % trim_left_copy(str);

	trim_right(str);
	cout << fmt % str;

	string str2 = "2010 Happy new Year!!!";
	cout << fmt % trim_left_copy_if(str2, is_digit());
	cout << fmt % trim_right_copy_if(str2, is_punct());
	cout << fmt % trim_copy_if(str2, is_punct() || is_digit() || is_space());
}

void str_find_usage()
{
	format fmt("|%s|.pos = %d\n");
	string str = "Long long ago,there was a king.";

	iterator_range<string::iterator> rge;

	rge = find_first(str, "long");
	cout << fmt % rge % (rge.begin() - str.begin());

	rge = ifind_first(str, "long");
	cout << fmt % rge % (rge.begin() - str.begin());

	rge = find_nth(str, "ng", 2);
	cout << fmt % rge % (rge.begin() - str.begin());

	rge = find_head(str,4);
	cout << fmt % rge % (rge.begin() - str.begin());

	rge = find_tail(str,5);
	cout << fmt % rge % (rge.begin() - str.begin());

	rge = find_first(str, "samus");
	assert(rge.empty() && !rge);
}

void replace_erase_usage()
{
	string str = "Samus beat the monster.\n";

	cout << replace_first_copy(str, "Samus", "samus");

	replace_last(str,"beat","kill");
	cout << str;

	replace_tail(str,9,"ridley.\n");
	cout << str;

	cout << ierase_all_copy(str, "samus");
	cout << replace_nth_copy(str,"l",1,"L");
	cout << erase_tail_copy(str, 8);
}

void string_split_usage()
{
	string str = "Samus,Link.Zelda::Mario-Luigi+zelda";
	deque<string> d;

	ifind_all(d,str,"zELDA");
	assert(d.size() == 2);

	for each (string str in d)
	{
		cout << "["<< str << "]";
	}
	cout << endl;

	list<iterator_range<string::iterator> > l;
	split(l, str, is_any_of(",.:-+"));
	for each (auto var in l)
	{
		cout << "[" << var << "]";
	}

	cout << endl;
	l.clear();

	split(l,str,is_any_of(".:-"),token_compress_on);
	for each (auto var in l)
	{
		cout << "[" << var << "]";
	}

	cout << endl;
}

#include <boost/assign.hpp>
void join_usage()
{
	using namespace boost::assign;
	vector<string> v = list_of("Samus")("Link")("Zelda")("Mario");
	cout << join(v, "+") << endl;

	struct is_contains_a
	{
		bool operator()(const string &x)
		{
			return contains(x, "a");
		}
	};
	cout << join_if(v,"**",is_contains_a());
}

void find_split_iterator_usage()
{
	string str("Samus||samus||mario||||Link");

	typedef find_iterator<string::iterator> string_find_iterator;

	string_find_iterator pos, end;
	for (pos = make_find_iterator(str, first_finder("samus", is_equal()));
		pos != end; ++pos
		)
	{
		cout << "[" << *pos << "]";
	}
	cout << endl;

	typedef split_iterator<string::iterator> string_split_iterator;

	string_split_iterator p, endp;
	for (p = make_split_iterator(str, first_finder("||", is_equal()));
		p != endp;++p)
	{
		cout << "[" << *p << "]";
	}
	cout << endl;
}

#include <boost/tokenizer.hpp>
void tokenizer_usage()
{
	string str("Link raise the master-sword");

	tokenizer<> tok(str);
	for each (string var in tok)
	{
		cout << "[" << var << "]";
	}
}

template<typename T>
void printt(T &tok)
{
	for each (auto var in tok)
	{
		cout << "[" << var << "]";
	}
	cout << endl;
}

void split_func_usage()
{
	char *str = "Link ;;<master-sword> zelda";
	char_separator<char> sep;
	tokenizer<char_separator<char>, char*> tok(str, str + strlen(str), sep);
	printt(tok);

	tok.assign(str, str + strlen(str), char_separator<char>(" ;-", "<>"));
	printt(tok);

	tok.assign(str, str + strlen(str), char_separator<char>(" ;-<>", "",keep_empty_tokens));
	printt(tok);

	string str1 = "id,100,name,\"mario\"";
	escaped_list_separator<char> sep1;
	tokenizer<escaped_list_separator<char> > tok1(str1,sep1);
	printt(tok1);

	string str2 = "2233344445";
	int offsets[] = {2,3,4};
	offset_separator sep2(offsets,offsets + 3,true,false);
	tokenizer<offset_separator> tok2(str2, sep2);
	printt(tok2);

	tok2.assign(str2, offset_separator(offsets, offsets + 3, false));
	printt(tok2);

	str2 += "56667";
	tok2.assign(str2, offset_separator(offsets, offsets + 3,true,false));
	printt(tok2);
}

#include <boost/xpressive/xpressive_dynamic.hpp>

void xpressive_usage()
{
	using namespace boost::xpressive;

	cregex reg = cregex::compile("a.c");
	assert(regex_match("abc",reg));
	assert(regex_match("a+c", reg));
	assert(!regex_match("ac",reg));
	assert(!regex_match("abd", reg));

	cregex reg1 = cregex::compile("\\d{6}(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{3}(X|\\d)", icase);
	assert(regex_match("999555197001019999",reg1));
	assert(regex_match("99955519700101999X", reg1));
	assert(regex_match("99955519700101999x", reg1));

	assert(!regex_match("99955520100101999Z", reg1));
	assert(!regex_match("99955530100101999X", reg1));
	assert(!regex_match("999555201099019998", reg1));
	assert(!regex_match("999555201012419991", reg1));

	cregex reg2 = cregex::compile("\\d{6}((1|2)\\d{3})((0|1)\\d)([0-3]\\d)(\\d{3}(X|\\d))", icase);
	cmatch what;
	assert(regex_match("999555197001019999",what,reg2));
	for each (auto var in what)
	{
		cout << "[" << var << "]" << endl;
	}
	cout << endl;

	cout <<"date:" << what[1] << what[3] << what[5] << endl;

	string str("readme.txt");

	sregex start_reg = sregex::compile("^re.*");
	sregex end_reg = sregex::compile(".*txt$");

	assert(regex_match(str,start_reg));
	assert(regex_match(str,end_reg));
}

void regex_search_usage()
{
	using namespace boost::xpressive;
	
	char* str = "there is a POWER-suit item";
	cregex reg = cregex::compile("(power)-(.{4})",icase);

	assert(regex_search(str,reg));

	cmatch what;
	regex_search(str,what,reg);
	assert(what.size() == 3);

	cout << what[1] << what[2] << endl;
	assert(!regex_search("err message",reg));

	string str1("readme.TXT");

	sregex start_reg = sregex::compile("^re");
	sregex end_reg  = sregex::compile("txt$",icase);

	assert(regex_search(str1,start_reg));
	assert(regex_search(str1,end_reg));
	assert(regex_search(str1,sregex::compile("me")));
}

void regex_replace_usage()
{
	using namespace boost::xpressive;

	string str("readme.txt");

	sregex reg1 = sregex::compile("(.*)(me)");
	sregex reg2 = sregex::compile("(t)(.)(t)");

	cout << regex_replace(str, reg1, "mamual") << endl;
	cout << regex_replace(str, reg1, "$1you") << endl;
	cout << regex_replace(str, reg1, "$&$&") << endl;
	cout << regex_replace(str, reg2, "$1N$3") << endl;

	str = regex_replace(str,reg2,"$1$3");
	cout << str << endl;

	string str2("2010 Happy new Year!!!");

	sregex reg11 = sregex::compile("^(\\d| )*");
	sregex reg22 = sregex::compile("!*$");

	cout << regex_replace(str2,reg11,"") << endl;
	cout << regex_replace(str2,reg22,"") << endl;

	str2 = regex_replace(str2,reg11,"Y2000 ");
	cout << str2 << endl;
}

void regex_iterator_usage()
{
	using namespace boost::xpressive;

	string str("Power-bomb,power-suit,pOWER-beam all items\n");

	sregex reg = sregex::compile("power-(\\w{4})",icase);

	sregex_iterator pos(str.begin(), str.end(), reg);
	sregex_iterator end;
	while (pos != end)
	{
		cout << "[" << (*pos)[0] << "]";
		++pos;
	}
	cout << endl;
}

void regex_token_iterator_usage()
{
	using namespace boost::xpressive;

	char* str = "*Link*||+Mario+||Zelda!!!||Metroid";

	cregex reg = cregex::compile("\\w+", icase);

	cregex_token_iterator pos(str, str + strlen(str), reg);
	while (pos != cregex_token_iterator())
	{
		cout << "[" << *pos << "]";
		++pos;
	}
	cout << endl;

	cregex split_reg = cregex::compile("\\|\\|");
	pos = cregex_token_iterator(str, str + strlen(str), split_reg, -1);

	while (pos != cregex_token_iterator())
	{
		cout << "[" << *pos << "]";
		++pos;
	}
	cout << endl;
}

template<class T, typename C>
std::vector<T> SplitChar(T& str, C c)
{
	std::vector<T> temp;
	typedef std::basic_stringstream<C, std::char_traits<C>, std::allocator<C> > mstringstream;
	mstringstream mss;
	mss << str;
	T s;
	while (getline(mss, s, c))
	{
		temp.push_back(s);
	}
	return temp;
}

void string_test()
{
#if 0
	lexical_cast_usage();
	format_usage();
	string_algo_usage();
	trim_usage();
	str_find_usage();
	replace_erase_usage();
	string_split_usage();
	join_usage();
	find_split_iterator_usage();
	tokenizer_usage();
	split_func_usage();
	xpressive_usage();
	regex_search_usage();
	regex_replace_usage();
	regex_iterator_usage();
#endif		
	regex_token_iterator_usage();
	SplitChar(string("aa,bb,cc,dd"), ',');
}

#endif //LEE_STRING