export module TR.Essentials.Clock;

export import <chrono>;

export namespace TR {

	namespace Essentials {

		struct Clock {

			using steady_clock = std::chrono::steady_clock;

			struct Duration : public steady_clock::duration {

				template <typename Rep>
				Rep Seconds() const {
					return std::chrono::duration_cast<std::chrono::duration<Rep>>(*this).count();
				}

			};

			Duration Elapsed() {
				return Duration(steady_clock::now() - start);
			}

			Duration Restart() {
				auto now = steady_clock::now();
				Duration duration(now - start);
				start = now;
				return duration;
			}

		protected:

			steady_clock::time_point start = steady_clock::now();

		};

	}
	using namespace Essentials;

}