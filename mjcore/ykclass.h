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
				class HAN { // �|
				public:
					HAN() { han = 0; unit = Han; }
					HAN(int8_t h) { han = h; unit = Han; }
					HAN(int8_t h, hanUnit u) { han = h; unit = u; }
					int8_t getHan() {return this->han;}
					hanUnit getUnit() {return this->unit;}
				private:
					int8_t han; // ���l
					hanUnit unit; // �P��
				};
				HAN coreHan; // ����𖞂����|
				HAN bonusHan; // ����𖞂����Ȃ��|
				YAKU_HAN() {coreHan = HAN(); bonusHan = HAN();}
				YAKU_HAN(HAN han) {coreHan = han; bonusHan = HAN();}
				YAKU_HAN(HAN han, HAN bonus) {coreHan = han; bonusHan = bonus;}
			};
		private:
			YAKU_HAN han;
			std::string yakuName; // ���̖��O�i������j
			YAKUFUNC yakuProc; // ���̔�����@
			Yaku() {} // Default constructor
		public:
			bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu) {
				 // ���𔻒肷��
				return this->yakuProc(gameStat, mentsu);
			}
			std::string getName() {return this->yakuName;} // ���̖��O���擾����
			YAKU_HAN getHan() {return han;}
			// Constructor
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan, YAKUFUNC f) {
				yakuName = std::string(name.begin(), name.end());
				han = YAKU_HAN(cHan, bHan);
				yakuProc = YAKUFUNC(f);
			}
		};

	}
}
#endif
