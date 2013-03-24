<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="4.0" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" />

	<!-- ルート要素 -->
	<xsl:template match="/">
		<html lang="ja-JP">
			<head>
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
		</h2>
		<p>結果：<xsl:value-of select="round-description/result" /></p>
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
</xsl:stylesheet>
