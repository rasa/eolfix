<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">
  <xsl:include href="/usr/share/xml/docbook/stylesheet/docbook-xsl/html/docbook.xsl"/>

  <xsl:param name="chapter.autolabel" select="1" /> 
  <xsl:param name="section.autolabel" select="1" /> 
  <xsl:param name="section.label.includes.component.label" select="1" /> 

  <xsl:param name="html.stylesheet" select="'eolfix.css'"/>
</xsl:stylesheet>
