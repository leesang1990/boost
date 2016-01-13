#ifndef LEE_THREAD
#define LEE_THREAD

#include <iostream>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

template<typename T>
class basic_atom :noncopyable
{
private:
	T n;
	typedef boost::mutex mutex_t;
	mutex_t mu;
public:
	basic_atom(T x = T()) :n(x){}
	T operator++()
	{
		mutex_t::scoped_lock lock(mu);
		return ++n;
	}
	operator T(){ return n; }
};

typedef basic_atom<int> atom_int;

boost::mutex io_mu;

void end_msg(const string& msg)
{
	cout << msg << endl;
}

void printing(atom_int& x, const std::string& str)
{
	for (int i = 0; i < 5; ++i)
	{
		boost::mutex::scoped_lock lock(io_mu);
		cout << str << ++x << endl;
	}
	this_thread::at_thread_exit(std::bind(end_msg, "end"));
}

void thread_usage()
{
	atom_int x;

	thread t1(printing,boost::ref(x),"hello");
	thread t2(printing, boost::ref(x), "boost");

	//this_thread::sleep(posix_time::seconds(2));
	t1.timed_join(posix_time::seconds(1));
	t2.detach();
	t2.join();

	thread t3(std::bind(printing,boost::ref(x),"thread"));
	std::function<void()> f = std::bind(printing, boost::ref(x), "mutex");
	thread t4(f);

	cout << t4.get_id() << endl;
	assert(t4 != t3);
	assert(t2.get_id() == thread::id());

	thread::sleep(get_system_time() + posix_time::seconds(1));
	cout << thread::hardware_concurrency() << endl;

	this_thread::sleep(posix_time::seconds(2));
	cout << this_thread::get_id();
	this_thread::yield();
}

void to_interrupt(atom_int& x, const string& str)
{
	try
	{
		for (int i = 0; i < 5; ++i)
		{
			this_thread::disable_interruption di;
			assert(!this_thread::interruption_enabled());
			//1.this_thread::sleep(posix_time::seconds(1));
			boost::mutex::scoped_lock lock(io_mu);
			cout << str << ++x << endl;
			
			cout << this_thread::interruption_requested() << endl;
			this_thread::interruption_point();

			this_thread::restore_interruption ri(di);
			assert(this_thread::interruption_enabled());
			cout << "can interrupt" << endl;
			cout << this_thread::interruption_requested() << endl;
			this_thread::interruption_point();
		}
		assert(this_thread::interruption_enabled());
	}
	catch (thread_interrupted&)
	{
		cout << "thread_interrupted" << endl;
	}
}

void interrupt_usage()
{
	atom_int x;
	thread t(to_interrupt, std::ref(x), "hello");
	//1.this_thread::sleep(posix_time::seconds(2));
	t.interrupt();
	t.join();
}

void thread_group_usage()
{
	atom_int x;
	thread_group tg;
	tg.create_thread(std::bind(printing, std::ref(x), "C++"));
	tg.create_thread(std::bind(printing,std::ref(x),"boost"));
	tg.join_all();
}

#include <stack>
class buffer
{
private:
	boost::mutex mu;
	condition_variable_any cond_put;
	condition_variable_any cond_get;
	stack<int> stk;
	int un_read, capacity;
	bool is_full(){ return un_read == capacity; }
	bool is_empty(){ return un_read == 0; }
public:
	buffer(size_t n) :un_read(0), capacity(n){}
	void put(int x)
	{
		{
			boost::mutex::scoped_lock lock(mu);
			while (is_full())
			{
				{
					boost::mutex::scoped_lock lock(io_mu);
					cout << "full waiting..." << endl;
				}
				cond_put.wait(mu);
			}
			stk.push(x);
			++un_read;
		}
		cond_get.notify_one();
	}

	void get(int *x)
	{
		{
			boost::mutex::scoped_lock lock(mu);
			while (is_empty())
			{
				{
					boost::mutex::scoped_lock lock(io_mu);
					cout << "empty waiting..." << endl;
				}
				cond_get.wait(mu);
			}
			--un_read;
			*x = stk.top();
			stk.pop();
		}
		cond_put.notify_one();
	}
};

buffer buf(5);
void producer(int n)
{
	for (int i = 0; i < n; ++i)
	{
		{
			boost::mutex::scoped_lock lock(io_mu);
			cout << "put " << i << endl;
		}
		buf.put(i);
	}
}

void consumer(int n)
{
	int x;
	for (int i = 0; i < n; ++i)
	{
		buf.get(&x);
		boost::mutex::scoped_lock lock(io_mu);
		cout << "get " << x << endl;
	}
}

void producer_and_consumer()
{
	thread t1(producer, 20);
	thread t2(consumer, 10);
	thread t3(consumer, 10);

	t1.join();
	t2.join();
	t3.join();
}

class rw_data
{
private:
	int m_x;
	shared_mutex rw_mu;
public:
	rw_data() :m_x(0){}
	void write()
	{
		unique_lock<shared_mutex> ul(rw_mu);
		++m_x;
	}
	void read(int *x)
	{
		shared_lock<shared_mutex> sl(rw_mu);
		*x = m_x;
	}
};

void writer(rw_data &d)
{
	for (int i = 0; i < 20;i++)
	{
		this_thread::sleep(posix_time::millisec(10));
		d.write();
	}
}

void reader(rw_data &d)
{
	int x;
	for (int i = 0; i < 10;++i)
	{
		this_thread::sleep(posix_time::millisec(5));
		d.read(&x);
		boost::mutex::scoped_lock lock(io_mu);
		cout << "reader:" << x << endl;
	}
}

void shared_mutex_usage()
{
	rw_data d;
	thread_group pool;

	pool.create_thread(std::bind(reader, std::ref(d)));
	pool.create_thread(std::bind(reader, std::ref(d)));
	pool.create_thread(std::bind(reader, std::ref(d)));
	pool.create_thread(std::bind(reader, std::ref(d)));
	pool.create_thread(std::bind(writer, std::ref(d)));
	pool.create_thread(std::bind(writer, std::ref(d)));

	pool.join_all();
}

int fab(int n)
{
	return (n == 0 || n == 1) ? 1 : fab(n - 1) + fab(n - 2);
}

void future_usage()
{
	packaged_task<int> pt(std::bind(fab, 10));

	unique_future<int> uf = pt.get_future();

	thread(boost::move(pt));
	uf.wait();
	assert(uf.is_ready() && uf.has_value());
	cout << uf.get();

	//多个
	typedef packaged_task<int> pti_t;
	typedef unique_future<int> ufi_t;

	std::array<pti_t, 5> ap;
	std::array<ufi_t, 5> au;

	for (int i = 0; i < 5; ++i)
	{
		ap[i] = pti_t(std::bind(fab, i + 10));
		au[i] = ap[i].get_future();
		thread(std::move(ap[i]));
	}

	wait_for_all(au.begin(), au.end());

	for (int i = 0; i < 5; ++i)
	{
		cout << au[i].get() << endl;
	}
}

void fab2(int n, promise<int> *p)
{
	p->set_value(fab(n));
}

void promise_usage()
{
	promise<int> p;
	unique_future<int> uf = p.get_future();

	thread(fab2, 10, &p);

	uf.wait();
	cout << uf.get() << endl;
	
}

#include <boost/detail/lightweight_mutex.hpp>

static int g_count;
boost::once_flag of = BOOST_ONCE_INIT;

void init_count()
{
	cout << "should call once." << endl;
	g_count = 0;
}

void call_func()
{
	boost::call_once(of, init_count);
}

//barrier 护栏 同步线程
barrier br(5);

void printing2(atom_int& x)
{
	{
		boost::mutex::scoped_lock lock(io_mu);
		cout << "thread " << ++x << " arrived barrier." << endl;
	}
	br.wait();

	boost::mutex::scoped_lock lock(io_mu);
	cout << "thread run." << endl;
}

//线程本地存储 编写可重入函数
void printing3()
{
	thread_specific_ptr<int> pi;
	pi.reset(new int(10));

	++(*pi);
	boost::mutex::scoped_lock lock(io_mu);
	cout << "thread v=" << *pi << endl;
}

//lazy future
void lazy_call_back(packaged_task<int>& task)
try
{
	task();
}
catch (task_already_started&)
{
}

void advance_usage()
{
	//lightweight_mutex
	boost::mutex mu1, mu2;
	lock(mu1, mu2);

	mu1.unlock();
	mu2.unlock();

	//call_once
	(thread(call_func));
	(thread(call_func));

	this_thread::sleep(posix_time::seconds(1));

	atom_int x;
	thread_group tg;

	for (int i = 0; i < 5;++i)
	{
		tg.create_thread(std::bind(printing2, std::ref(x)));
	}
	tg.join_all();

	//线程本地存储
	(thread(printing3));
	(thread(printing3));
	this_thread::sleep(posix_time::seconds(1));

	//lazy future
	packaged_task<int> task(bind(fab, 10));
	task.set_wait_callback(lazy_call_back);
	unique_future<int> uf = task.get_future();

	cout << uf.get();
}

void thread_test()
{
#if 0
	thread_usage();
	interrupt_usage();
	thread_group_usage();
	producer_and_consumer();
	shared_mutex_usage();
	future_usage();
	promise_usage();
#endif			
	advance_usage();
	
}

#endif