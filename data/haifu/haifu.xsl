<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet [
	<!ENTITY east "&#x1f000;"><!ENTITY south "&#x1f001;"><!ENTITY west "&#x1f002;"><!ENTITY north "&#x1f003;"><!ENTITY red "&#x1f004;"><!ENTITY green "&#x1f005;"><!ENTITY white "&#x1f006;">
	<!ENTITY m1 "&#x1f007;"><!ENTITY m2 "&#x1f008;"><!ENTITY m3 "&#x1f009;"><!ENTITY m4 "&#x1f00a;"><!ENTITY m5 "&#x1f00b;"><!ENTITY m6 "&#x1f00c;"><!ENTITY m7 "&#x1f00d;"><!ENTITY m8 "&#x1f00e;"><!ENTITY m9 "&#x1f00f;">
	<!ENTITY s1 "&#x1f010;"><!ENTITY s2 "&#x1f011;"><!ENTITY s3 "&#x1f012;"><!ENTITY s4 "&#x1f013;"><!ENTITY s5 "&#x1f014;"><!ENTITY s6 "&#x1f015;"><!ENTITY s7 "&#x1f016;"><!ENTITY s8 "&#x1f017;"><!ENTITY s9 "&#x1f018;">
	<!ENTITY p1 "&#x1f019;"><!ENTITY p2 "&#x1f01a;"><!ENTITY p3 "&#x1f01b;"><!ENTITY p4 "&#x1f01c;"><!ENTITY p5 "&#x1f01d;"><!ENTITY p6 "&#x1f01e;"><!ENTITY p7 "&#x1f01f;"><!ENTITY p8 "&#x1f020;"><!ENTITY p9 "&#x1f021;">
	<!ENTITY plum "&#x1f022;"><!ENTITY orchid "&#x1f023;"><!ENTITY bamboo "&#x1f024;"><!ENTITY chrys "&#x1f025;"><!ENTITY chrysanthemum "&#x1f025;"><!ENTITY spring "&#x1f026;"><!ENTITY summer "&#x1f027;"><!ENTITY autumn "&#x1f028;"><!ENTITY winter "&#x1f029;"><!ENTITY joker "&#x1f02a;"><!ENTITY back "&#x1f02b;">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="4.0" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" />

	<!-- ルート要素 -->
	<xsl:template match="/">
		<html lang="ja-JP">
			<head>
				<title><xsl:value-of select="haifu/match-description/title" /></title>
				<link rel="stylesheet" href="haifu.css" type="text/css" />
			</head>
			<body>
				<xsl:apply-templates />
			</body>
		</html>
	</xsl:template>

	<!-- 牌譜メタデータ -->
	<xsl:template match="/haifu/match-description">
		<h1><xsl:value-of select="title" /></h1>
	</xsl:template>

	<!-- 局進行データ -->
	<xsl:template match="/haifu/round">
		<hr />
		<h2>
			<!--東一局 ドラ：-->
			<xsl:call-template name="wind-name">
				<xsl:with-param name="wind" select="round-description/round-number/@prevailing-wind" />
			</xsl:call-template>
			<xsl:call-template name="chinese-numeral">
				<xsl:with-param name="val" select="round-description/round-number/@rotation" />
			</xsl:call-template>
			<xsl:text>局</xsl:text>
			<xsl:if test="round-description/round-number/@counter > 0">
				<xsl:text> </xsl:text>
				<xsl:value-of select="round-description/round-number/@counter" />
				<xsl:text>本場</xsl:text>
			</xsl:if>
			<xsl:if test="round-description/dora">
				<xsl:text> ドラ：</xsl:text>
				<span class="tile">
					<xsl:for-each select="round-description/dora/tile">
						<xsl:call-template name="tile">
							<xsl:with-param name="tile" select="@tile" />
							<xsl:with-param name="dora" select="@dora" />
						</xsl:call-template>
					</xsl:for-each>
				</span>
			</xsl:if>
			<xsl:if test="round-description/uradora">
				<xsl:text> 裏ドラ：</xsl:text>
				<span class="tile">
					<xsl:for-each select="round-description/uradora/tile">
						<xsl:call-template name="tile">
							<xsl:with-param name="tile" select="@tile" />
							<xsl:with-param name="dora" select="@dora" />
						</xsl:call-template>
					</xsl:for-each>
				</span>
			</xsl:if>
			<xsl:if test="round-description/alice">
				<xsl:text> アリス：</xsl:text>
				<span class="tile">
					<xsl:for-each select="round-description/alice/tile">
						<xsl:call-template name="tile">
							<xsl:with-param name="tile" select="@tile" />
							<xsl:with-param name="dora" select="@dora" />
						</xsl:call-template>
					</xsl:for-each>
				</span>
			</xsl:if>
		</h2>
		<xsl:if test="round-description/dice">
			<p>
				<xsl:text>サイコロ :</xsl:text>
				<xsl:for-each select="round-description/dice">
					<xsl:text> </xsl:text>
					<span class="dice">
						<xsl:call-template name="dice-face">
							<xsl:with-param name="val" select="@face1" />
						</xsl:call-template>
						<xsl:call-template name="dice-face">
							<xsl:with-param name="val" select="@face2" />
						</xsl:call-template>
					</span>
				</xsl:for-each>
			</p>
		</xsl:if>
		<p>結果：<xsl:value-of select="round-description/result" /></p>
		<table>
			<xsl:variable name="cols" select="40" />
			<xsl:for-each select="round-description/player-score/player">
				<xsl:variable name="pcode" select="@ref" />
				<!-- 幅の調整 -->
				<tr>
					<xsl:call-template name="width-adjuster">
						<xsl:with-param name="iteration" select="$cols" />
					</xsl:call-template>
				</tr>
				<!-- プレイヤー別の情報 -->
				<tr>
					<td class="player" colspan="{$cols}">
						<xsl:call-template name="wind-name">
							<xsl:with-param name="wind" select="@wind" />
						</xsl:call-template>
						<xsl:text>家 </xsl:text>
						<xsl:value-of select="/haifu/match-description/player-description/player-data[@id = $pcode]/@name" />
						<xsl:text> </xsl:text>
						<xsl:call-template name="stylize-sign">
							<xsl:with-param name="val" select="@score" />
						</xsl:call-template>
						<xsl:if test="@delta">
							<xsl:text> → </xsl:text>
							<xsl:call-template name="stylize-sign">
								<xsl:with-param name="val" select="number(@score)+number(@delta)" />
							</xsl:call-template>
							<xsl:text> (</xsl:text>
							<xsl:call-template name="stylize-sign">
								<xsl:with-param name="val" select="@delta" />
							</xsl:call-template>
							<xsl:text>)</xsl:text>
						</xsl:if>
						<xsl:if test="@chip">
							<xsl:text> チップ：</xsl:text>
							<xsl:call-template name="stylize-sign">
								<xsl:with-param name="val" select="@chip" />
							</xsl:call-template>
						</xsl:if>
					</td>
				</tr>
				<!-- プレイヤー別の情報：配牌 -->
				<tr>
					<td class="label">配牌</td>
					<td colspan="{$cols - 1}">
						<span class="tile">
							<xsl:for-each select="../../../distribution/initial-hand[@player = $pcode]/tile">
								<xsl:call-template name="tile">
									<xsl:with-param name="tile" select="@tile" />
									<xsl:with-param name="dora" select="@dora" />
								</xsl:call-template>
							</xsl:for-each>
						</span>
					</td>
				</tr>
				<!-- プレイヤー別の情報：自摸 -->
				<tr class="tile">
					<td class="label" rowspan="2">自摸</td>
					<xsl:for-each select="../../../progress/cycle">
						<td>
							<xsl:if test="turn[@player = $pcode]">
								<xsl:if test="turn[@player = $pcode]/pick">
									<xsl:call-template name="tile">
										<xsl:with-param name="tile" select="turn[@player = $pcode]/pick/@tile" />
										<xsl:with-param name="dora" select="turn[@player = $pcode]/pick/@dora" />
									</xsl:call-template>
								</xsl:if>
								<xsl:if test="turn[@player = $pcode]/discard-through">
									<span class="fallthru">↓</span>
								</xsl:if>
							</xsl:if>
						</td>
					</xsl:for-each>
				</tr>
				<tr>
					<xsl:for-each select="../../../progress/cycle">
						<td class="notice">
							<xsl:if test="turn[@player = $pcode]">
								<xsl:if test="turn[@player = $pcode]/pick">
									<xsl:variable name="meld" select="turn[@player = $pcode]/pick/@meld" />
									<xsl:choose>
										<xsl:when test="$meld = 'mahjong' ">ツモ</xsl:when>
										<xsl:when test="$meld = 'sequence'">チー</xsl:when>
										<xsl:when test="$meld = 'triplet' ">ポン</xsl:when>
										<xsl:when test="$meld = 'quad'    ">カン</xsl:when>
										<xsl:when test="$meld = 'flower'  ">花</xsl:when>
									</xsl:choose>
								</xsl:if>
							</xsl:if>
						</td>
					</xsl:for-each>
				</tr>
				<!-- プレイヤー別の情報：打牌 -->
				<tr class="tile">
					<td class="label" rowspan="2">打牌</td>
					<xsl:for-each select="../../../progress/cycle">
						<td>
							<xsl:if test="turn[@player = $pcode]">
								<xsl:if test="turn[@player = $pcode]/discard">
									<xsl:call-template name="tile">
										<xsl:with-param name="tile" select="turn[@player = $pcode]/discard/@tile" />
										<xsl:with-param name="dora" select="turn[@player = $pcode]/discard/@dora" />
									</xsl:call-template>
								</xsl:if>
								<xsl:if test="turn[@player = $pcode]/discard-through">
									<xsl:call-template name="tile">
										<xsl:with-param name="tile" select="turn[@player = $pcode]/discard-through/@tile" />
										<xsl:with-param name="dora" select="turn[@player = $pcode]/discard-through/@dora" />
									</xsl:call-template>
								</xsl:if>
							</xsl:if>
						</td>
					</xsl:for-each>
				</tr>
				<tr>
					<xsl:for-each select="../../../progress/cycle">
						<td class="notice">
							<xsl:if test="turn[@player = $pcode]">
								<xsl:if test="turn[@player = $pcode]/discard">
									<xsl:variable name="meld" select="turn[@player = $pcode]/discard/@call" />
									<xsl:variable name="let-go" select="turn[@player = $pcode]/discard/@let-go" />
									<xsl:choose>
										<xsl:when test="$meld = 'richi' ">リーチ<br /></xsl:when>
										<xsl:when test="$meld = 'quad'  ">カン<br /></xsl:when>
										<xsl:when test="$meld = 'flower'">花<br /></xsl:when>
									</xsl:choose>
									<xsl:if test="$let-go = 'true'">ウチ<br /></xsl:if>
								</xsl:if>
								<xsl:if test="turn[@player = $pcode]/discard-through">
									<xsl:variable name="meld" select="turn[@player = $pcode]/discard-through/@call" />
									<xsl:variable name="let-go" select="turn[@player = $pcode]/discard-through/@let-go" />
									<xsl:choose>
										<xsl:when test="$meld = 'richi' ">リーチ<br /></xsl:when>
										<xsl:when test="$meld = 'quad'  ">カン<br /></xsl:when>
										<xsl:when test="$meld = 'flower'">花<br /></xsl:when>
									</xsl:choose>
									<xsl:if test="$let-go = 'true'">ウチ<br /></xsl:if>
								</xsl:if>
							</xsl:if>
						</td>
					</xsl:for-each>
				</tr>
				<!-- プレイヤー別の情報：最終形 -->
				<tr>
					<td class="label">牌姿</td>
					<td colspan="{$cols - 1}">
						<xsl:for-each select="../../../final-hands/final-hand[@player = $pcode]">
							<xsl:apply-templates mode="final" />
						</xsl:for-each>
					</td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- 牌を表示 -->
	<xsl:template name="tile">
		<xsl:param name="tile" />
		<xsl:param name="dora" select="normal" />
		<xsl:choose>
			<xsl:when test="$dora = 'red'">
				<span class="akadora">
					<xsl:value-of select="$tile" />
				</span>
			</xsl:when>
			<xsl:when test="$dora = 'blue'">
				<span class="aodora">
					<xsl:value-of select="$tile" />
				</span>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$tile" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="tile-rotated">
		<xsl:param name="tile" />
		<xsl:param name="dora" select="normal" />
		<span class="rotated-tiles">
			<span class="rotated">
				<xsl:call-template name="tile">
					<xsl:with-param name="tile" select="$tile" />
					<xsl:with-param name="dora" select="$dora" />
				</xsl:call-template>
			</span>
		</span>
	</xsl:template>
	<xsl:template name="tile-rotated-double">
		<xsl:param name="tile" />
		<xsl:param name="dora" select="normal" />
		<xsl:param name="tile2" select="$tile" />
		<xsl:param name="dora2" select="normal" />
		<span class="rotated-tiles-dbl">
			<span class="rotated-stacked">
				<xsl:call-template name="tile">
					<xsl:with-param name="tile" select="$tile" />
					<xsl:with-param name="dora" select="$dora" />
				</xsl:call-template>
				<xsl:call-template name="tile">
					<xsl:with-param name="tile" select="$tile2" />
					<xsl:with-param name="dora" select="$dora2" />
				</xsl:call-template>
			</span>
		</span>
	</xsl:template>

	<!-- 最終牌姿表示用 -->
	<xsl:template match="hand" mode="final">
		<span class="tile">
			<xsl:for-each select="tile">
				<xsl:call-template name="tile">
					<xsl:with-param name="tile" select="@tile" />
					<xsl:with-param name="dora" select="@dora" />
				</xsl:call-template>
			</xsl:for-each>
		</span>
	</xsl:template>
	<xsl:template match="finishing-tile" mode="final">
		<xsl:choose>
			<xsl:when test="@finish-type = 'ron'  "><xsl:text> ロン</xsl:text></xsl:when>
			<xsl:when test="@finish-type = 'tsumo'"><xsl:text> ツモ</xsl:text></xsl:when>
			<xsl:otherwise                         ><xsl:text> 和了</xsl:text></xsl:otherwise>
		</xsl:choose>
		<span class="tile">
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="tile/@tile" />
				<xsl:with-param name="dora" select="tile/@dora" />
			</xsl:call-template>
		</span>
	</xsl:template>
	<xsl:template match="flower" mode="final">
		<xsl:text> 花</xsl:text>
		<span class="tile">
			<xsl:for-each select="tile">
				<xsl:call-template name="tile">
					<xsl:with-param name="tile" select="@tile" />
				</xsl:call-template>
			</xsl:for-each>
		</span>
	</xsl:template>
	<xsl:template match="quad-concealed" mode="final">
		<xsl:text> カン</xsl:text>
		<span class="tile">
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="'&back;'" />
			</xsl:call-template>
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
				<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
			</xsl:call-template>
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
				<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
			</xsl:call-template>
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="'&back;'" />
			</xsl:call-template>
		</span>
	</xsl:template>
	<xsl:template match="sequence" mode="final">
		<xsl:text> チー</xsl:text>
		<span class="tile">
			<xsl:call-template name="tile-rotated">
				<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
				<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
			</xsl:call-template>
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
				<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
			</xsl:call-template>
			<xsl:call-template name="tile">
				<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
				<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
			</xsl:call-template>
		</span>
	</xsl:template>
	<xsl:template match="triplet" mode="final">
		<xsl:text> ポン</xsl:text>
		<span class="tile">
			<xsl:choose>
				<xsl:when test="@meld-direction = 'left'">
					<xsl:call-template name="tile-rotated">
						<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="@meld-direction = 'opposite'">
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile-rotated">
						<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="@meld-direction = 'right'">
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile">
						<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
					</xsl:call-template>
					<xsl:call-template name="tile-rotated">
						<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
						<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</span>
	</xsl:template>
	<xsl:template match="quad-exposed" mode="final">
		<xsl:text> カン</xsl:text>
		<span class="tile">
			<xsl:choose>
				<xsl:when test="@added = 'true'">
					<xsl:choose>
						<xsl:when test="@meld-direction = 'left'">
							<xsl:call-template name="tile-rotated-double">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
								<xsl:with-param name="tile2" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora2" select="tile[position() = 2]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test="@meld-direction = 'opposite'">
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile-rotated-double">
								<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
								<xsl:with-param name="tile2" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora2" select="tile[position() = 3]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test="@meld-direction = 'right'">
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile-rotated-double">
								<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
								<xsl:with-param name="tile2" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora2" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="@meld-direction = 'left'">
							<xsl:call-template name="tile-rotated">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test="@meld-direction = 'opposite'">
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile-rotated">
								<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test="@meld-direction = 'right'">
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 1]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 1]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 2]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 2]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile">
								<xsl:with-param name="tile" select="tile[position() = 3]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 3]/@dora" />
							</xsl:call-template>
							<xsl:call-template name="tile-rotated">
								<xsl:with-param name="tile" select="tile[position() = 4]/@tile" />
								<xsl:with-param name="dora" select="tile[position() = 4]/@dora" />
							</xsl:call-template>
						</xsl:when>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<!-- 幅調整用 -->
	<xsl:template name="width-adjuster">
		<xsl:param name="iteration" />
		<xsl:param name="index" select="1" />
		<td width="{100.0 div $iteration}%" />
		<xsl:if test="$index &lt; $iteration">
			<xsl:call-template name="width-adjuster">
				<xsl:with-param name="iteration" select="$iteration" />
				<xsl:with-param name="index" select="$index + 1" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- 風位を英日翻訳 -->
	<xsl:template name="wind-name">
		<xsl:param name="wind" />
		<xsl:choose>
			<xsl:when test="$wind = 'east' ">東</xsl:when>
			<xsl:when test="$wind = 'south'">南</xsl:when>
			<xsl:when test="$wind = 'west' ">西</xsl:when>
			<xsl:when test="$wind = 'north'">北</xsl:when>
			<xsl:when test="$wind = 'white'">白</xsl:when>
			<xsl:when test="$wind = 'green'">發</xsl:when>
			<xsl:when test="$wind = 'red'  ">中</xsl:when>
			<xsl:otherwise><xsl:value-of select="$wind" /></xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- 漢数字に変換（１〜１０） -->
	<xsl:template name="chinese-numeral">
		<xsl:param name="val" />
		<xsl:choose>
			<xsl:when test="$val =  1" >一</xsl:when>
			<xsl:when test="$val =  2" >ニ</xsl:when>
			<xsl:when test="$val =  3" >三</xsl:when>
			<xsl:when test="$val =  4" >四</xsl:when>
			<xsl:when test="$val =  5" >五</xsl:when>
			<xsl:when test="$val =  6" >六</xsl:when>
			<xsl:when test="$val =  7" >七</xsl:when>
			<xsl:when test="$val =  8" >八</xsl:when>
			<xsl:when test="$val =  9" >九</xsl:when>
			<xsl:when test="$val = 10" >十</xsl:when>
			<xsl:otherwise><xsl:value-of select="$val" /></xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- 符号の表記 -->
	<xsl:template name="stylize-sign">
		<xsl:param name="val" />
		<xsl:choose>
			<xsl:when test="$val > 0" >
				<xsl:text>+</xsl:text>
				<xsl:value-of select="$val" />
			</xsl:when>
			<xsl:when test="$val &lt; 0" >
				<xsl:text>△</xsl:text>
				<xsl:value-of select="substring-after($val, '-')" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$val" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- サイコロの目（１〜６） -->
	<xsl:template name="dice-face">
		<xsl:param name="val" />
		<xsl:choose>
			<xsl:when test="$val =  1" >&#x2680;</xsl:when>
			<xsl:when test="$val =  2" >&#x2681;</xsl:when>
			<xsl:when test="$val =  3" >&#x2682;</xsl:when>
			<xsl:when test="$val =  4" >&#x2683;</xsl:when>
			<xsl:when test="$val =  5" >&#x2684;</xsl:when>
			<xsl:when test="$val =  6" >&#x2685;</xsl:when>
			<xsl:otherwise><xsl:value-of select="$val" /></xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
