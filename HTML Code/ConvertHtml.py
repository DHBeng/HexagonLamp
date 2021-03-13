def insertValue(line: str, value: str, hexValue: bool):
    valueStartPos = 0
    temp1 = ""
    temp2 = ""
    temp3 = ""
    result = []
    
    valueStartPos = line.find("\"temp")
    if (hexValue):
        temp1 = "client.print(\"" + line[0:valueStartPos+1] +"#\");\n"
    else:
        temp1 = "client.print(\"" + line[0:valueStartPos+1] +"\");\n"
    temp2 = "client.print(" + value + ");\n"
    temp3 = "client.println(\"" + line[valueStartPos+5:] + "\");\n"
    result = [temp1, temp2, temp3]

    return result

def find_nth(string: str, subString : str, n: int):
    start = string.find(subString)
    while start >= 0 and n > 1:
        start = string.find(subString, start+len(subString))
        n -= 1
    return start

def buildSwitchCase(options: str):
    numberOptions = options.count("<option")
    option = ""
    commandList = [None] * 100
    index = 0
    commandList[index] = "switch (WebVisu::mode)"
    index = index + 1
    commandList[index] = "{"
    index = index + 1
    for i in range(1, numberOptions + 1):
        commandList[index] = "case " + str(i) + ":"
        index = index + 1
        for j in range(1, numberOptions + 1):
            if (j != numberOptions):
                option = options[find_nth(options, "<option", j):find_nth(options, "<option", j + 1)]
            else:
                option = options[find_nth(options, "<option", j):]
            if (i == j):
                commandList[index] = "\tclient.println(\"" + option[0:8] + "selected " + option[8:] + "\");"
                index = index + 1
            else:
                commandList[index] = "\tclient.println(\"" + option + "\");"
                index = index + 1
        commandList[index] = "\tbreak;"
        index = index + 1
    commandList[index] = "}"
    return commandList


sourceFile = open("WebVisu.html", "r")
htmlCode = sourceFile.readlines()
htmlForArduino = ""
for line in htmlCode:
    line = line.replace("    ", "")
    line = line.replace("\n", "")
    line = line.replace("\"", "\\\"")
    htmlForArduino = htmlForArduino + line
sourceFile.close()

destinatioFile = open("WebVisu.txt", "w")
htmlLine = ""
j = 0
for i in htmlForArduino:
    htmlLine = htmlLine + i
    j = j + 1
    if j >= 80:
        if (i == ">") or (i == "}"):
            if (htmlLine.find("<select") >= 0):
                temp = []
                if (htmlLine.endswith("</select>")):
                    selectedPos = htmlLine.find("<select")
                    optionPos = htmlLine.find("<option")
                    line = "client.println(\"" + htmlLine[0:selectedPos] + "\");\n"
                    destinatioFile.write(line)
                    line = "client.println(\"" + htmlLine[selectedPos:optionPos] + "\");\n"
                    destinatioFile.write(line)
                    temp = buildSwitchCase(htmlLine[optionPos:])
                    for line in temp:
                        if (line == None):
                            break
                        destinatioFile.write(line + "\n")
                    j = 0
                    htmlLine = ""
                else:
                    j = j + 1
            elif (htmlLine.find("name") >= 0 and htmlLine.find("temp") >= 0):
                temp = []
                if (htmlLine.find("name=\\\"M") >= 0):
                    modeCnt = htmlForArduino.count("<option")
                    temp = insertValue(htmlLine, "WebVisu::mode", False)
                elif (htmlLine.find("name=\\\"B") >= 0):
                    temp = insertValue(htmlLine, "WebVisu::brightness", False)
                elif (htmlLine.find("name=\\\"S") >= 0):
                    temp = insertValue(htmlLine, "WebVisu::speed", False)
                elif (htmlLine.find("name=\\\"C") >= 0):
                    temp = insertValue(htmlLine, "WebVisu::colorHex, HEX", True)
                else:
                    print("!!! Error: Input not defind !!!")
                destinatioFile.writelines(temp)
                j = 0
                htmlLine = ""
            else:
                htmlLine = "client.println(\"" + htmlLine + "\");\n"
                destinatioFile.write(htmlLine)
                j = 0
                htmlLine = ""
        else:
            j = j + 1
            
if j > 0:
    htmlLine = "client.println(\"" + htmlLine + "\");"
    destinatioFile.write(htmlLine )

print("--- File created ---")

destinatioFile.close()