destinatioFile = open("WebVisu.cpp", "r")
cppCode = destinatioFile.readlines()
destinatioFile.close()
cppCodeModified = [None] * 1000
i = 0
copy = True
destinatioFile = open("WebVisu.cpp", "w")
for line in cppCode:
    if line.find("---AUTO GENERATED CODE - END ---") >= 0:
        copy = True
    if copy == True:
        destinatioFile.write(line)
        cppCodeModified[i] = line
        i = i + 1
    if line.find("---AUTO GENERATED CODE - START ---") >= 0:
        copy = False
        numberTabs = line.count("    ")
        leadingSpace = ""
        for i in range(0, numberTabs):
            leadingSpace = leadingSpace + "    "
        sourceFile = open("HtmlCode\WebVisu.txt", "r")
        htmlCode = sourceFile.readlines()
        sourceFile.close()
        for generatedLine in htmlCode:
            destinatioFile.write(leadingSpace + generatedLine)
            cppCodeModified[i] = leadingSpace + generatedLine
            i = i + 1
destinatioFile.close()