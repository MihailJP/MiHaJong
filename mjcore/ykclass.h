#ifndef YKCLASS_H
#define YKCLASS_H

#include <string>
#include <functional>
#include <cstdint>
#include "gametbl.h"

namespace yaku {
	namespace yakuCalculator {

		struct MENTSU_ANALYSIS;

		typedef std::function<bool
			(const GameTable* const, const MENTSU_ANALYSIS* const)> YAKUFUNC;

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
			bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu) {
				 // ���𔻒肷��
				return this->yakuProc(gameStat, mentsu);
			}
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
