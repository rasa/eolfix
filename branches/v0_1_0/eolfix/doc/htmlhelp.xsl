<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">
  <xsl:include href="/usr/share/docbook-xsl/htmlhelp/htmlhelp.xsl"/>
  
  <xsl:param name="chapter.autolabel" select="1" /> 
  <xsl:param name="section.autolabel" select="1" /> 
  <xsl:param name="section.label.includes.component.label" select="1" /> 
  
  <xsl:param name="generate.legalnotice.link" select="1"/>
  <xsl:param name="suppress.navigation" select="0"/>
  <xsl:param name="admin.graphics" select="1"/>
  <xsl:param name="admin.graphics.path">gfx/</xsl:param>
  <xsl:param name="html.stylesheet" select="'eolfix.css'"/>
  <xsl:param name="htmlhelp.chm" select="'eolfix-ug.chm'"/>
  <xsl:param name="htmlhelp.hhc.binary" select="0"/>
  <xsl:param name="htmlhelp.hhc.folders.instead.books" select="0"/>
  <xsl:param name="toc.section.depth" select="4"/>
  <xsl:template name="user.header.navigation">
  </xsl:template>
</xsl:stylesheet>
