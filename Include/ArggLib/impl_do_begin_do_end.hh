#ifndef ArggLib_impl_do_begin_do_end_h__
#define ArggLib_impl_do_begin_do_end_h__
#include "ArggLib/has_member_helper.hh"

namespace ArggLib {

	namespace ArggLib_impl {



		CREATE_TEST_FOR_MEMBER(hasEnd, End());
		CREATE_TEST_FOR_MEMBER(hasStart, Start());

		CREATE_TEST_FOR_MEMBER(has_t, t);
		CREATE_TEST_FOR_MEMBER(has_n, n);



		template <typename N>
		procReturn do_end1(N&& n) {
			return success;
		}

		template <typename N>
		__ENABLE_IF(hasEnd<N>(), procReturn) do_end1(N& n) {
			return	n.End();

		}

		template <typename N>
		__ENABLE_IF(hasEnd<const N>(), procReturn) do_end1(const N& n) {
			return	n.End();

		}

		template <typename N>
		__ENABLE_IF(hasEnd< N>(), procReturn) do_end1(N&& n) {
			return n.End();

		}


		template <typename N>
		__ENABLE_IF_NOT(has_n<N>(), procReturn)  do_end2(N&& n) {
			return do_end1(n);
		}

		template <typename N>
		__ENABLE_IF(has_n<N>(), procReturn) do_end2(N& n) {
			return	do_end1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n<const N>(), procReturn) do_end2(const N& n) {
			return	do_end1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n< N>(), procReturn) do_end2(N&& n) {
			return	do_end1(n.n);

		}


		template <typename P>
		procReturn unfold_end(P&& p) {
			do_end2(p);
			return success;
		}

		template <typename P>
		__ENABLE_IF(has_t<P>(), procReturn)  unfold_end(P& p) {

			unfold_end(p.t);
			return do_end2(p);
		}


		template <typename P>
		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_end(const P& p) {

			unfold_end(p.t);
			return do_end2(p);
		}


		template <typename P>
		__ENABLE_IF(has_t< P>(), procReturn)  unfold_end(P&& p) {

			unfold_end(p.t);
			return do_end2(p);
		}






		template <typename N>
		procReturn do_Start1(N&& n) {
			return success;
		}

		template <typename N>
		__ENABLE_IF(hasStart<N>(), procReturn) do_Start1(N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart<const N>(), procReturn) do_Start1(const N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart< N>(), procReturn) do_Start1(N&& n) {
			return n.Start();

		}



		template <typename N>
		__ENABLE_IF_NOT(has_n<N>(), procReturn)  do_Start2(N&& n) {
			return do_Start1(n);
		}

		template <typename N>
		__ENABLE_IF(has_n<N>(), procReturn) do_Start2(N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n<const N>(), procReturn) do_Start2(const N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n< N>(), procReturn) do_Start2(N&& n) {
			return	do_Start1(n.n);

		}


		template <typename P>
		procReturn unfold_Start(P&& p) {
			do_Start2(p);
			return success;
		}

		template <typename P>
		__ENABLE_IF(has_t<P>(), procReturn)  unfold_Start(P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_Start(const P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t< P>(), procReturn)  unfold_Start(P&& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}

	}

}



#endif // ArggLib_impl_do_begin_do_end_h__
