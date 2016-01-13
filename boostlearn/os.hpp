#ifndef LEE_IO
#define LEE_IO

#include <iostream>
#include <fstream>
#include <boost/io/ios_state.hpp>

using namespace std;
using namespace boost::io;

void logging(const char*msg)
{
	cout << msg << endl;
}

void io_state_saver_usage()
{
	string filename("logfile.txt");

	cout << "log start" << endl;
	if (!filename.empty())
	{
		ofstream fs(filename.c_str());
		ios_all_saver ifs(cout);
		cout.rdbuf(fs.rdbuf());
		logging("fatal msg");
	}
	cout << "log finish" << endl;
}

#include <boost/progress.hpp>
#include <boost/static_assert.hpp>

template<int N = 2>
class new_progress_timer :public boost::timer
{
public:
	new_progress_timer(ofstream &os = std::cout) :m_os(os)
	{
		BOOST_STATIC_ASSERT(N >= 0 && N <= 10);
	}
	~new_progress_timer(void)
	{
		try
		{
			//std::istream::fmtflags old_flags = m_os.setf(istream::fixed, istream::floatfield);
			//std::streamsize old_prec = m_os.precision(N);
			ios_all_saver ios(m_os);

			m_os.setf(istream::fixed, istream::floatfield);
			m_os.precision(N);
			m_os << elapsed() << "s\n" << endl;

			//m_os.flags(old_flags);
			//m_os.precision(old_prec);
		}
		catch (...){}
	}

private:
	ofstream &m_os;
};

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
using namespace boost::system;

class my_category :public boost::system::error_category
{
public:
	virtual const char *name() const
	{
		return "myapp_category";	
	}
	virtual string message(int ev) const
	{
		string msg;
		switch (ev)
		{
		case 0:
			msg = "ok";
			break;
		default:
			msg = "some error";
			break;
		}
		return msg;
	}
};

void my_category_usage()
{
	my_category my_cat;
	boost::system::error_code ec(10, my_cat);
	cout << ec.value() << ec.message() << endl;
	ec = boost::system::error_code(10, boost::system::system_category());
	cout << ec.value() << ec.message() << endl;

	cout << boost::system::system_category().name() << endl;

	boost::system::error_code ecc;
	assert(ecc.value() == boost::system::errc::success);
	assert(!ecc);

	assert(ecc.category() == boost::system::system_category());

	ecc.assign(3L, boost::system::system_category());
	boost::system::error_condition econd = ecc.default_error_condition();

	assert(econd == boost::system::system_category().default_error_condition(3L));

	cout << ecc.message() << endl;
	cout << econd.message() << endl;
	cout << econd.value() << endl;

	try
	{
		throw boost::system::system_error::system_error(boost::system::error_code(5, boost::system::system_category()));
	}
	catch (boost::system::system_error& e)
	{
		cout << e.what();
	}

}

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

void path_usage()
{
	path p1("./a_dir");
	path p2("/usr/local/lib");

	path p3("c:\\tmp\\test.text");
	path p4("d:/boost/boost/filesystem/");

	path p5;
	assert(p5.empty());

	char str[] = "the path is (/root).";
	path p(str + 13,str + 14);

	assert(!p.empty());

	p /= "etc";
	string filename = "xinetd.conf";
	p.append(filename.begin(),filename.end());
	cout << p << endl;

	cout << system_complete(p) << endl;

	string fname("w+abc.xxx");
	assert(!portable_posix_name(fname));
	assert(windows_name(fname));

	assert(!portable_name("w+()abc.txt")&&!portable_name("./abc"));
	assert(!portable_directory_name("a.txt") && portable_directory_name("abc"));
	assert(portable_file_name("a.bc")&&!portable_file_name("y.conf"));
}

void path_usage1()
{
	path p("/usr/local/include/xxx.hpp");

	cout << p.string() << endl;
	cout << p.generic_string() << endl;
	cout << p.parent_path() << endl;
	cout << p.stem() << endl;
	cout << p.filename() << endl;
	cout << p.extension() << endl;

	assert(!p.is_complete());
	assert(system_complete(p).is_complete());

	cout << p.root_name() << endl;
	cout << p.root_directory() << endl;
	cout << p.root_path() << endl;

	assert(!p.has_root_name());
	assert(p.has_root_path());
	assert(p.has_parent_path());

	cout << p.replace_extension() << endl;
	cout << p.replace_extension("hxx") << endl;
	cout << p.remove_filename() << endl;

	path p1("c:/test/1.cpp");
	path p2("c:/TEST/1.cpp");
	path p3("c:/abc/1.cpp");

	assert(p1 != p2);
	assert(p2 < p3);

	path pp = "/boost/tools/libs";

	auto pos = pp.begin();
	while (pos != pp.end())
	{
		cout << "[" << *pos << "]";
		++pos;
	}

	path pppp("c:/test.txt");

	try
	{
		file_size(p);
	}
	catch (filesystem_error& e)
	{
		cout << e.path1() << endl;
		cout << e.what() << endl;
	}

}

#include <boost/filesystem/operations.hpp>

void file_status_usage()
{
	assert(status("d:/Git").type() == directory_file);
	assert(status("d:/boost/nofile").type() == file_not_found);
	assert(status("d:/boost/README.txt").type() != symlink_file);
	assert(status("d:/Git/README.txt").type() != regular_file);

	assert(!is_directory("d:/boost"));
	assert(!exists("d:/boost/nofile"));
	assert(!is_symlink("d:/boost/README.txt"));
	assert(!is_other("d:/boost/README.txt"));
	assert(!is_regular_file("d:/boost/README.txt"));
	assert(!boost::filesystem::is_empty("d:/BugReport.txt"));
}

void file_properties_usage()
{
	cout << initial_path() << endl;
	cout << current_path() << endl;

	path p("d:/BugReport.txt");
	cout << file_size(p) << endl;
	time_t t = last_write_time(p);
	last_write_time(p,time(0));

	const int GBYTES = 1024 * 1024 * 1024;
	space_info si = space("d:/");
	cout << si.capacity / GBYTES << endl;
	cout << si.available / GBYTES << endl;
	cout << si.free / GBYTES << endl;
}

void recursive_dir(const path& dir)
{
	directory_iterator end;
	for (directory_iterator pos(dir); pos != end; ++pos)
	{
		if (is_directory(*pos))
		{
			recursive_dir(*pos);
		}
		else
		{
			cout << *pos << endl;
		}
	}
}

void file_operate_usage()
{
	namespace fs = boost::filesystem;
	path ptest = "c:/test";
	if (exists(ptest))
	{
		if (fs::is_empty(ptest))
		{
			fs::remove(ptest);
		}
		else
		{
			remove_all(ptest);
		}
	}

	assert(!exists(ptest));
	create_directory(ptest);

	copy_file("d:/BugReport.txt",ptest / "a.txt");
	assert(exists(ptest / "a.txt"));
	rename(ptest / "a.txt", ptest / "b.txt");
	assert(exists(ptest / "b.txt"));

	create_directories(ptest / "sub_dir1" / "sub_dir1");

	recursive_dir("c:/test");

	typedef recursive_directory_iterator rd_iterator;

	rd_iterator end;
	for (rd_iterator pos("c:/test"); pos != end;++pos)
	{
		cout << "level" << pos.level() << ":" << *pos << endl;
	}

	rd_iterator end1;
	for (rd_iterator pos("c:/test"); pos != end1; ++pos)
	{
		if (is_directory(*pos))
		{
			pos.no_push();
		} 
		cout << *pos << endl;
	}
}

#include <boost/optional.hpp>

optional<path> find_file(const path& dir, const string& filename)
{
	typedef optional<path> result_type;
	typedef recursive_directory_iterator rd_iterator;

	if (!exists(dir) || !is_directory(dir))
	{
		return result_type();
	}

	rd_iterator end;
	for (rd_iterator pos(dir); pos != end;++pos)
	{
		if (!is_directory(*pos)&& pos->path().filename() == filename)
		{
			return result_type(pos->path());
		}
	}
	return result_type();
}

#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::xpressive;

void find_files(const path& dir, const string& filename, vector<path>& v)
{
	static xpressive::sregex_compiler rc;
	if (!rc[filename].regex_id())
	{
		string str = replace_all_copy(
			replace_all_copy(filename,".","\\."),
			"*",".*");
		rc[filename] = rc.compile(str);
	}

	typedef vector<path> result_type;
	typedef recursive_directory_iterator rd_iterator;

	if (!exists(dir)||!is_directory(dir))
	{
		return;
	}

	rd_iterator end;
	for (rd_iterator pos(dir); pos != end;++pos)
	{
		if (!is_directory(*pos) &&
			regex_match(pos->path().filename().generic_string(),rc[filename]))
		{
			v.push_back(pos->path());
		}
	}
}

#include <boost/progress.hpp>
size_t copy_files(const path& from_dir, const path& to_dir,
	const string& filename = "*")
{
	if (!is_directory(from_dir))
	{
		cout << "args is not a dir." << endl;
		return 0;
	}
	cout << "prepare for copy,please wait..." << endl;

	vector<path> v;
	find_files(from_dir, filename, v);

	if (v.empty())
	{
		cout << "0 file copied." << endl;
		return 0;
	}

	cout << "now begin copy files ..." << endl;
	path tmp;
	progress_display pd(v.size());
	for each (auto p in v)
	{
		tmp = to_dir / p.string().substr(from_dir.string().length());
		if (!exists(tmp.parent_path()))
		{
			create_directories(tmp.parent_path());
		}
		if (!exists(tmp))
		{
			copy_file(p, tmp);
		}	
		++pd;
	}
	cout << v.size() << " file copied." << endl;
	return v.size();
}

#include <boost/filesystem/fstream.hpp>
namespace newfs = boost::filesystem;

void find_file_test()
{
// 	optional<path> r = find_file("c:/test", "b.txt");
// 	if (r)
// 	{
// 		cout << *r << endl;
// 	}
// 	else
// 	{
// 		cout << "file not found." << endl;
// 	}

	vector<path> v;
	find_files("d:/atest", "*.txt", v);
	cout << v.size() << endl;

	for each (auto p in v)
	{
		cout << p << endl;
	}
	copy_files("d:/atest", "d:/aatest");

	path p("d:/atest/BugReport.txt");
	newfs::ifstream ifs(p.string().c_str());
	assert(ifs.is_open());
	cout << ifs.rdbuf();
}

#include <boost/program_options.hpp>
using namespace boost::program_options;

int program_option_usage(int argc, _TCHAR* argv[])
{
	options_description opts("demo options");

	opts.add_options()("help", "just a help info")
		("filename", program_options::value<string>(), "to find a file");
	variables_map vm;
	store(parse_command_line(argc, argv, opts), vm);

	if (vm.count("help"))
	{
		cout << opts << endl;
		return 0;
	}

	if (vm.count("filename"))
	{
		cout << "find" << vm["filename"].as<string>() << endl;
	}
	if (vm.size() == 0)
	{
		cout << "no options" << endl;
	}
	return 1;
}

void print_vm(options_description &opts, variables_map &vm)
{
	if (vm.size() == 0)
	{
		cout << opts << endl;
		return;
	}

	if (vm.count("help"))
	{
		cout << opts << endl;
	}

	cout << "find opt:" << vm["filename"].as<string>() << endl;

	if (vm.count("dir"))
	{
		cout << "dir opt:";
		for each (auto str in vm["dir"].as<vector<string> >())
		{
			cout << str << ",";
		}
		cout << endl;
	}

	if (vm.count("depth"))
	{
		cout << "depth opt:" << vm["depth"].as<int>() << endl;
	}
}

//boostlearn.exe --filename abc
//boostlearn.exe -f readme.txt --dir c: d: -d

#include <boost/assign.hpp>
string name_mapper(const string& env_name)
{
	using namespace boost::assign;
	static map<string, string> nm =
		map_list_of("TMP", "tmp")("USERNAME", "uname");

	return nm[env_name];
}

void option_description_usage(int argc, _TCHAR* argv[])
{
	options_description opts("demo options");

	string filename;
	opts.add_options()
		("help,h", "help message\n a bit of long text")
		("filename,f",program_options::value<string>(&filename)->default_value("test"),"to find a file")
		("dir,D",program_options::value<vector<string> >()->multitoken(),"search dir")
		("depth,d",program_options::value<int>()->implicit_value(5),"search depth")
		("tmp", program_options::value<string>(),"tmp var")
		("uname", program_options::value<string>(),"user's name");


	variables_map vm;
	store(parse_command_line(argc,argv,opts),vm);
	notify(vm);

	positional_options_description pod;
	pod.add("filename",1);
	auto pr = wcommand_line_parser(argc, argv).options(opts).positional(pod).allow_unregistered().run();
	store(pr,vm);

	store(parse_environment(opts, name_mapper), vm);
	cout << vm["tmp"].as<string>() << endl;
	cout << vm["uname"].as<string>() << endl;

	print_vm(opts, vm);
}

#include <boost/filesystem/fstream.hpp>

void parse_config_usage()
{
	options_description opts("lee");
	boost::filesystem::fstream ifs("config.txt");
	variables_map vm;
	store(parse_config_file(ifs,opts,true),vm);

	char *str = "config.ini";
	store(parse_config_file<char>(str,opts,true),vm);
}

void os_test(int argc, _TCHAR* argv[])
{
#if 0
	io_state_saver_usage();
	my_category_usage();
	path_usage();
	path_usage1();
	file_status_usage();
	file_properties_usage();
	file_operate_usage();
	find_file_test();
	program_option_usage(argc, argv);
	parse_config_usage();
	
#endif	
	option_description_usage(argc, argv);

}

#endif//LEE_IO
