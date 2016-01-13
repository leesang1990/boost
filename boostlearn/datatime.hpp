#ifndef LEE_DATETIME
#define LEE_DATETIME

#include <iostream>
#include <boost/timer.hpp>

using namespace std;
using namespace boost;

void timer_usage()
{
	timer t;

	cout << "max timespan:" << t.elapsed_max() / 3600 << "h" << endl;
	cout << "min timespan:" << t.elapsed_min() / 3600 << "s" << endl;

	cout << "now time elapsed:" << t.elapsed() << "s" << endl;
}

#include <boost/progress.hpp>

void progress_usage()
{
	progress_timer t;

	cout << t.elapsed() << endl;

	stringstream ss;
	{
		progress_timer t(ss);
	}
	cout<< ss.str();
}

#include <boost/static_assert.hpp>

void progress_display_usage()
{
	vector<string> v(100,"aaa");
	std::ofstream fs("test.txt");

	v[10] = ""; v[23] = "";
	progress_display pd(v.size());

	for (auto pos = v.begin(); pos != v.end();++pos)
	{
		fs << *pos << endl;
		//++pd;
		pd.restart(v.size());
		pd += (pos - v.begin() + 1);
		if (pos->empty())
		{
			cout << "null string #" << (pos - v.begin()) << endl;
		}
	}
}

//#define  DATE_TIME_NO_DEFAULT_CONSTRUCTOR         // cancel default constructor
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

void date_usage()
{
	date d1;
	date d2(2010,4,1);
	date d3(2000, Jan, 1);
	date d4(d2);

	cout << to_simple_string(d2) << endl;
	cout << to_iso_string(d2) << endl;
	cout << to_iso_extended_string(d2) << endl;
	cout << d2 << endl;

	assert(d1 == date(not_a_date_time));
	assert(d2 == d4);
	assert(d3 < d4);

	date d5 = from_string("1999-12-31");
	date d6(from_string("2005/1/1"));
	date d7 = from_undelimited_string("20011118");

	cout << day_clock::local_day() << endl;
	cout << day_clock::universal_day() << endl;

	date d8(neg_infin);
	date d9(pos_infin);
	date d10(not_a_date_time);
	date d11(max_date_time);
	date d12(min_date_time);

	assert(d8.is_infinity());
	assert(d8.is_neg_infinity());
	assert(d9.is_pos_infinity());
	assert(d10.is_not_a_date());
	assert(d10.is_special());

	cout<<d8<<endl<<d9<<endl<<d10<<endl<<d11<<endl<<d12<<endl;

	//date d13(1399, 12, 1);				//超过日期下限
	//date d14(10000, 1, 1);				//超过日期上限
	//date d15(2010, 2, 29);				//不存在的日期

	assert(d2.year() == 2010);
	assert(d2.month() == 4);
	assert(d2.day() == 1);

	date::ymd_type ymd = d2.year_month_day();
	assert(ymd.year == 2010);
	assert(ymd.month == 4);
	assert(ymd.day == 1);

	assert(d2.day_of_week() == 4);
	assert(d2.day_of_year() == 91);
	assert(d2.end_of_month() == date(2010,4,30));

	assert(date(2010,1,10).week_number() == 1);
	assert(date(2010,1,1).week_number() == 53);
	assert(date(2008,1,1).week_number() == 1);

	date d16(2010,2,1);
	tm t = to_tm(d16);
	assert(t.tm_hour == 0 && t.tm_min == 0);
	assert(t.tm_year == 110 && t.tm_mday == 1);

	date d17 = date_from_tm(t);
	assert(d16 == d17);
}

void date_duration_usge()
{
	days dd1(10), dd2(-100), dd3(255);

	assert(dd1 > dd2 && dd1 < dd3);
	assert(dd1 + dd2 == days(-90));
	assert((dd1 + dd3).days() == 265);
	assert(dd3 / 5 == days(51));

	weeks w(3);
	assert(w.days() == 21);

	months m(5);
	years y(2);

	months m2 = y + m;
	assert(m2.number_of_months() == 29);
	assert((y*2).number_of_years() == 4);
}

void date_operate()
{
	date d1(2000, 1, 1), d2(2008, 8, 8);
	cout << d2 - d1 << endl;
	assert(d1 + (d2 - d1) == d2);

	d1 += days(10);
	assert(d1.day() == 11);
	d1 += months(2);
	assert(d1.month() == 3 && d1.day() == 11);
	d1 -= weeks(1);
	assert(d1.day() == 4);

	d2 -= years(7);
	assert(d2.year() == d1.year() + 1);

	date d3(2010,1,1);

	date d4 = d3 + days(pos_infin);
	assert(d4.is_pos_infinity());

	d4 = d3 + days(not_a_date_time);
	assert(d4.is_not_a_date());
	d2 = date(neg_infin);
	days dd = d1 - d2;
	assert(dd.is_special() && !dd.is_negative());

	date d5(2010,3,30);
	d5 -= months(1);			//2010-2-28
	d5 -= months(1);			//2010-1-31
	d5 += months(2);			//2010-3-31
	assert(d5.day() == 31);     //use Undefined BOOST_DATE_TIME_OPTIONAL_GREGORIAN_TYPES
}

void date_period_usage()
{
	date_period dp1(date(2010,1,1),days(20));
	date_period dp2(date(2010, 1, 1), date(2009, 1, 1)); //无效
	date_period dp3(date(2010, 3, 1), days(-20));        //无效

	date_period dp(date(2010,1,1),days(20));

	assert(!dp.is_null());
	assert(dp.begin().day() == 1);
	assert(dp.last().day() == 20);
	assert(dp.end().day() == 21);
	assert(dp.length().days() == 20);

	date_period dp4(date(2010,1,1),days(20));
	date_period dp5(date(2010,2,19),days(10));

	cout<<dp4;
	assert(dp4 < dp5);
}

void date_period_operate()
{
	date_period dp(date(2010,1,1),days(20));

	dp.shift(days(3));
	assert(dp.begin().day() == 4);
	assert(dp.length().days() == 20);

	dp.expand(days(3));
	assert(dp.begin().day() == 1);
	assert(dp.length().days() == 26);

	date_period dp1(date(2010, 1, 1), days(20));
	assert(dp1.is_after(date(2009,12,1)));
	assert(dp1.is_before(date(2010,2,1)));
	assert(dp1.contains(date(2010,1,10)));

	date_period dp2(date(2010, 1, 5), days(10));
	assert(dp.contains(dp2));

	assert(dp.intersects(dp2));
	assert(dp.intersection(dp2) == dp2);

	date_period dp3(date(2010,1,21),days(5));
	assert(!dp3.intersects(dp2));
	assert(dp3.intersection(dp2).is_null());

	assert(dp.is_adjacent(dp3));
	assert(!dp.intersects(dp3));
}

void date_period_operate1()
{
	date_period dp1(date(2010, 1, 1), days(20));
	date_period dp2(date(2010, 1, 5), days(10));
	date_period dp3(date(2010, 2, 1), days(5));
	date_period dp4(date(2010, 1, 15), days(10));

	assert(dp1.contains(dp2) && dp1.merge(dp2) == dp1);
	assert(!dp1.intersects(dp3) && dp1.merge(dp3).is_null());
	assert(dp1.intersects(dp2) && dp1.merge(dp4).end() == dp4.end());
	assert(dp1.span(dp3).end() == dp3.end());

	date d(2006,11,26);
	day_iterator d_iter(d);

	assert(d_iter == d);
	++d_iter;
	assert(d_iter == date(2006,11,27));

	year_iterator y_iter(*d_iter,3);
	assert(y_iter == d + days(1));
	++y_iter;
	assert(y_iter->year() == 2009);

	typedef gregorian_calendar gre_cal;
	cout << "Y2010 is " << (gre_cal::is_leap_year(2010) ? "" : "not")
		<< " a leap year." << endl;
	assert(gre_cal::end_of_month_day(2010, 2) == 28);
}

void date_time_example1()
{
	date d(2008,11,20);
	date d_start(d.year(),d.month(),1);
	date d_end = d.end_of_month();

	for (day_iterator d_iter(d_start); d_iter != d_end;++d_iter)
	{
		cout << *d_iter << " " << d_iter->day_of_week() << endl;
	}
}

void date_time_example2()
{
	date d(2008, 11, 20);

	date d18years = d + years(18);
	cout << d18years << " is " << d18years.day_of_week() << endl;

	int count = 0;
	for (day_iterator d_iter(date(d18years.year(), 11, 1)); d_iter != d18years.end_of_month();++d_iter)
	{
		if (d_iter->day_of_week() == Sunday)
		{
			++count;
		}
	}
	cout<<"total "<<count<<" Sundays."<<endl;

	count = 0;
	for (month_iterator m_iter(date(d18years.year(), 1, 1)); m_iter < date(d18years.year() + 1, 1, 1);++m_iter)
	{
		count += m_iter->end_of_month().day();
	}
	cout<<"total "<<count<<" days of year."<<endl;
}

class credit_card
{
public:
	string bank_name;
	int bill_day_no;
	credit_card(const char* bname, int no) :bank_name(bname), bill_day_no(no){}

	int calc_free_days(date consume_day = day_clock::local_day()) const
	{
		date bill_day(consume_day.year(), consume_day.month(), bill_day_no);

		if (consume_day > bill_day)
		{
			bill_day += months(1);
		}

		return (bill_day - consume_day).days() + 20;
	}

	friend bool operator<(const credit_card& l, const credit_card& r)
	{
		return l.calc_free_days() < r.calc_free_days();
	}
};

void date_time_example3()
{
	credit_card a("A bank",25);
	credit_card b("B bank",12);

	credit_card tmp = std::max(a, b);
	cout << "You should use " << tmp.bank_name
		<<",free days = "<<tmp.calc_free_days()<<endl;
}

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

void time_duration_usage()
{
	time_duration td(1,10,30,1000);
	time_duration td1(1, 60, 60, 1000 * 1000 * 6 + 1000);
	hours h(1);
	minutes m(10);
	seconds s(30);
	millisec ms(1);

	time_duration td2 = h + m + s + ms;
	time_duration td3 = hours(2) + seconds(10);

	time_duration td4 = duration_from_string("1:10:30:001");
	time_duration td5(1, 10, 30, 1000);
	assert(td5.hours() == 1 && td5.minutes() == 10 && td5.seconds() == 30);
	assert(td5.total_seconds() == 1 * 3600 + 10*60 + 30);
	assert(td5.total_milliseconds() == td.total_seconds()*1000 + 1);
	assert(td5.fractional_seconds() == 1000);

	hours h1(-10);
	assert(h1.is_negative());
	
	time_duration h2 = h1.invert_sign();
	assert(!h2.is_negative() && h2.hours() == 10);

	time_duration td6(not_a_date_time);
	assert(td6.is_special() && td6.is_not_a_date_time());

	time_duration td7(neg_infin);
	assert(td7.is_negative() && td7.is_neg_infinity());

	time_duration td8 = hours(1);
	time_duration td9 = hours(2) + minutes(30);
	assert(td8 < td9);
	assert((td8 + td9).hours() == 3);
	assert((td8 - td9).is_negative());
	assert(td8 * 5 == td9 * 2);
	assert((td8/2).minutes() == td9.minutes());

	time_duration td10(1, 10, 30, 1000);
	cout << to_simple_string(td10) << endl;
	cout << to_iso_string(td10) << endl;
}

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
#define BOOST_DATE_TIME_SOURCE
#include <boost/date_time/posix_time/posix_time.hpp>

void time_precision_usage()
{
	time_duration td(1, 10, 30, 1000);
	cout << td;
	assert(td.total_milliseconds() == td.total_seconds()*1000);

	assert(td.fractional_seconds() == 1000);//fractional_seconds 小数部分 单位纳秒
	assert(time_duration::unit() * 1000 * 1000 * 1000 == seconds(1)); //最小单位
	//BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG 单位纳秒 \
	默认微妙
	assert(td.resolution() == date_time::nano);
	assert(td.num_fractional_digits() == 9);

	//自定义毫秒单位
	time_duration::tick_type my_millisec =  
		time_duration::ticks_per_second() / 1000;
	time_duration td1(1,10,30,10 *my_millisec); //10毫秒 0.01秒
}

void ptime_usage()
{
	ptime p(date(2010,3,5),hours(1)); //2010年3月5日凌晨1时
	ptime p1 = time_from_string("2010-3-5 01:00:00");
	ptime p2 = from_iso_string("20100305T010000");

	ptime p3 = second_clock::local_time();		//秒精度
	ptime p4 = microsec_clock::universal_time();//微秒精度
	cout<<p3<<p4<<endl;

	ptime p5(not_a_date_time);
	assert(p5.is_not_a_date_time());
	ptime p6(pos_infin);
	assert(p6.is_special() && p6.is_pos_infinity());

	ptime p7(date(2010,3,20),hours(12)+minutes(30));

	date d = p7.date();
	time_duration td = p7.time_of_day();
	assert(d.month() == 3 && d.day() == 20);
	assert(td.total_seconds() == 12 * 3600 + 30 *60);

	ptime p8 = p7 + hours(3);

	assert(p7 < p8);
	assert(p8 - p7 == hours(3));
	p8 += months(1);
	assert(p8.date().month() == 4);

	ptime p9(date(2010,2,14),hours(20));
	cout << to_simple_string(p9) << endl;
	cout << to_iso_string(p9) << endl;
	cout << to_iso_extended_string(p9) << endl;
}

#include <boost/date_time/filetime_functions.hpp>
using namespace boost::date_time::winapi;

void ptime_to_tm()
{
	ptime p(date(2010,2,14),hours(20));
	tm t = to_tm(p);
	assert(t.tm_year == 110 && t.tm_hour == 20);

	ptime p1 = from_time_t(std::time(0));
	assert(p1.date() == day_clock::local_day());

	FILETIME ft;
	ft.dwHighDateTime = 29725317;
	ft.dwLowDateTime = 3685122988UL;
	p1 = from_ftime<ptime>(ft);

	ptime p2(date(2010,2,27),hours(10));
	for (time_iterator t_iter(p2, minutes(10)); t_iter < p + hours(1); ++t_iter)
	{
		cout << *t_iter << endl;
	}
}

template<typename Clock = microsec_clock>
class basic_ptimer
{
public:
	basic_ptimer(){restart();}
	void restart(){ _start_time = Clock::local_time(); }
	void elapsed() const { cout << Clock::local_time() - _start_time; }
	~basic_ptimer(){ elapsed(); }
private:
	ptime _start_time;
};

typedef basic_ptimer<second_clock> sptimer;

class work_time
{
public:
	typedef map<time_period, string> map_t;

	void init()
	{
		ptime p(day_clock::local_day());

		map_ts[time_period(p, hours(9))] = "It's too early,just relax.\n";
		p += hours(9);
		map_ts[time_period(p, hours(3) + minutes(30))] = "It's AM,please work hard.\n";
		p += hours(3) + minutes(30);
		map_ts[time_period(p, hours(1))] = "It's lunch time,are you hungry?\n";
		p += hours(1);
		map_ts[time_period(p, hours(4) + minutes(30))] = "It's PM,ready to go home.\n";
		p += hours(4) + minutes(30);
		map_ts[time_period(p, hours(6))] = "Are you still working?you do need a rest.\n";

	}
	work_time()
	{
		init();
	}

	void greeting(const ptime& t)
	{
		map_t::iterator pos;
		for (pos = map_ts.begin(); pos != map_ts.end();++pos)
		{
			if (pos->first.contains(t))
			{
				cout << pos->second << endl;
				break;
			}
		}
	}
private:
	map_t map_ts;
};

void date_time_format()
{
	date d(2010,3,6);
	date_facet* dfacet = new date_facet("%Y年%m月%d日");
	cout.imbue(locale(cout.getloc(),dfacet));
	cout << d << endl;
	delete dfacet;
	dfacet = nullptr;

	time_facet *tfacet = new time_facet("%Y年%m月%d日%H点%M分%S%F秒");
	cout.imbue(locale(cout.getloc(),tfacet));
	cout<<ptime(d,hours(21) + minutes(50) + millisec(100)) << endl;
	delete tfacet;
	tfacet = nullptr;
}

#include <boost/date_time/local_time/local_time.hpp>
using namespace boost::local_time;

void local_time_usage()
{
	tz_database tz_db;
	{
		sptimer spt;

		tz_db.load_from_file("E://develop//boost_1_58_0//libs//date_time//data//date_time_zonespec.csv");
	}
	cout << endl;

	time_zone_ptr shz = tz_db.time_zone_from_region("Asia/Shanghai");

	time_zone_ptr nyz = tz_db.time_zone_from_region("America/New_York");

	cout<<shz->has_dst()<<endl;
	cout<<shz->std_zone_name()<<endl;

	local_date_time dt_bj(date(2008,1,7),
		hours(12),
		shz,
		false);
	cout<<dt_bj<<endl;

	time_duration flight_time = hours(15);

	dt_bj += flight_time;
	cout<<dt_bj<<endl;
	local_date_time dt_ny = dt_bj.local_time_in(nyz);
	cout << dt_ny;
}

void datatime_test()
{
#if 0 
	timer_usage();
	progress_usage(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     );
	progress_display_usage();
	date_usage();
	date_duration_usge();
	date_operate();
	date_period_usage();
	date_period_operate();
	date_period_operate1();
	date_time_example1();
	date_time_example2();
	date_time_example3();
	time_duration_usage();
	ptime_usage();
	ptime_to_tm();
	{
		sptimer spt;
		work_time wt;
		wt.greeting(second_clock::local_time());
	}

	date_time_format();
#endif			

	
	local_time_usage();
}

#endif //LEE_DATETIME