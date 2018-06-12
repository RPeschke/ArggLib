#ifndef reactive_signals_h__
#define reactive_signals_h__
#include "ArggLib/reactive_programming/reactive_backend.hh"
#include "ArggLib/reactive_programming/reactive_obj_base.hh"
#include "ArggLib/contains.hh"
namespace ArggLib {

	template <typename T>
	class reactive_signals : public reactive_obj_base{
	public:
		using type = T;
		reactive_signals(T val, reactive_backend* backend) : reactive_obj_base(backend), last(val), current(val), next(val)   {
			
			register_processor(reactive_processor_on_begin([this]() mutable { this->update(); }));
		}
		reactive_signals(T val) :reactive_signals ( std::move(val), &get_reactive_backend())  {}

		operator T() {
			return current;
		}
		T value() const {
			return current;
		}

		void update() {
			last = current;

			current = next;

		}
		void set(const T& val) {
			next = val;
			notify();
		}
		template <typename T1>
		void operator<<=(T1&& t) {
			next = std::forward<T>(t);
			notify();
			return *this;
		}
		reactive_signals<T>& operator=(T value) {

			next = std::move(value);
			notify();
			return *this;
		}
	private:
		T last, current, next;

	};
}

#endif // reactive_signals_h__
