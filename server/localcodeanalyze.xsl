<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >

    <xsl:template match="/">
        <html>
            <body>
                <h2>代码分析报告：</h2>
                <table border="3">
                    <tr bgcolor="#6495ED">
                        <th>错误码</th>
                        <th>路径</th>
                        <th>描述</th>
                        <th>所在函数</th>
                    </tr>
                    <xsl:for-each select="DEFECTS/DEFECT">
                    <tr>
                        <td><xsl:value-of select="DEFECTCODE"/></td>
                        <td><xsl:value-of select="SFA/FILEPATH"/> <xsl:value-of select="SFA/FILENAME"/>。 行数:<xsl:value-of select="SFA/LINE"/> ，列数：<xsl:value-of select="SFA/COLUMN"/> 。</td>
                        <td><xsl:value-of select="DESCRIPTION"/></td>
                        <td><xsl:value-of select="FUNCTION"/></td>
                    </tr>
                    </xsl:for-each>
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
