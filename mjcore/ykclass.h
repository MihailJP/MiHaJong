#ifndef YKCLASS_H
#define YKCLASS_H

#include <string>
#include <functional>
#include <cstdint>

namespace yaku {
	namespace yakuCalculator {

		typedef std::function<bool ()> YAKUFUNC;

		enum hanUnit : uint8_t {Han, Mangan, Yakuman};
		class Yaku {
		public:
			class YAKU_HAN {
			public:
				YAKU_HAN() { han = 0; unit = Han; }
				YAKU_HAN(int8_t h, hanUnit u) { han = h; unit = u; }
			private:
				int8_t han; // 数値
				hanUnit unit; // 単位
			};
		private:
			YAKU_HAN yakuHan; // 縛りを満たす翻
			YAKU_HAN incompleteHan; // 縛りを満たさない翻
			std::string yakuName; // 役の名前（文字列）
			YAKUFUNC yakuProc; // 役の判定方法
			Yaku() {} // Default constructor
		public:
			bool checkYaku() {return this->yakuProc();} // 役を判定する
			std::string getName() {return this->yakuName;} // 役の名前を取得する
			// Constructor
			Yaku(std::string name, YAKU_HAN han, YAKU_HAN icHan, YAKUFUNC f) {
				this->yakuName = std::string(name.begin(), name.end());
				this->yakuHan = han;
				this->incompleteHan = icHan;
				this->yakuProc = YAKUFUNC(f);
			}
		};

	}
}
#endif
