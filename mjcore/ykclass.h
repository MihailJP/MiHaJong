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
				int8_t han; // ���l
				hanUnit unit; // �P��
			};
		private:
			YAKU_HAN yakuHan; // ����𖞂����|
			YAKU_HAN incompleteHan; // ����𖞂����Ȃ��|
			std::string yakuName; // ���̖��O�i������j
			YAKUFUNC yakuProc; // ���̔�����@
			Yaku() {} // Default constructor
		public:
			bool checkYaku() {return this->yakuProc();} // ���𔻒肷��
			std::string getName() {return this->yakuName;} // ���̖��O���擾����
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
