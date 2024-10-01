SamacSys ECAD Model
1013858/1490336/2.50/14/3/Integrated Circuit

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r114.5_55"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.550) (shapeHeight 1.145))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "ADXL343BCCZRL" (originalName "ADXL343BCCZRL")
		(multiLayer
			(pad (padNum 1) (padStyleRef r114.5_55) (pt -1.098, 2.000) (rotation 90))
			(pad (padNum 2) (padStyleRef r114.5_55) (pt -1.098, 1.200) (rotation 90))
			(pad (padNum 3) (padStyleRef r114.5_55) (pt -1.098, 0.400) (rotation 90))
			(pad (padNum 4) (padStyleRef r114.5_55) (pt -1.098, -0.400) (rotation 90))
			(pad (padNum 5) (padStyleRef r114.5_55) (pt -1.098, -1.200) (rotation 90))
			(pad (padNum 6) (padStyleRef r114.5_55) (pt -1.098, -2.000) (rotation 90))
			(pad (padNum 7) (padStyleRef r114.5_55) (pt 0.000, -2.098) (rotation 0))
			(pad (padNum 8) (padStyleRef r114.5_55) (pt 1.098, -2.000) (rotation 90))
			(pad (padNum 9) (padStyleRef r114.5_55) (pt 1.098, -1.200) (rotation 90))
			(pad (padNum 10) (padStyleRef r114.5_55) (pt 1.098, -0.400) (rotation 90))
			(pad (padNum 11) (padStyleRef r114.5_55) (pt 1.098, 0.400) (rotation 90))
			(pad (padNum 12) (padStyleRef r114.5_55) (pt 1.098, 1.200) (rotation 90))
			(pad (padNum 13) (padStyleRef r114.5_55) (pt 1.098, 2.000) (rotation 90))
			(pad (padNum 14) (padStyleRef r114.5_55) (pt 0.000, 2.098) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0.000, 0.000) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 2.5) (pt 1.5 2.5) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 2.5) (pt 1.5 -2.5) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 -2.5) (pt -1.5 -2.5) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 -2.5) (pt -1.5 2.5) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.67 3.67) (pt 2.67 3.67) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 2.67 3.67) (pt 2.67 -3.67) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 2.67 -3.67) (pt -2.67 -3.67) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.67 -3.67) (pt -2.67 3.67) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.1 2) (pt -2.1 2) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -2.15, 2) (radius 0.05) (startAngle .0) (sweepAngle 180.0) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.2 2) (pt -2.2 2) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -2.15, 2) (radius 0.05) (startAngle 180.0) (sweepAngle 180.0) (width 0.1))
		)
	)
	(symbolDef "ADXL343BCCZ-RL" (originalName "ADXL343BCCZ-RL")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 5) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 6) (pt 0 mils -500 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -525 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 7) (pt 1000 mils -1100 mils) (rotation 90) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1025 mils -870 mils) (rotation 90]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 8) (pt 2000 mils -500 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -525 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 9) (pt 2000 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 10) (pt 2000 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 11) (pt 2000 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 12) (pt 2000 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 13) (pt 2000 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1770 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 14) (pt 1000 mils 800 mils) (rotation 270) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1025 mils 570 mils) (rotation 90]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 600 mils) (pt 1800 mils 600 mils) (width 6 mils))
		(line (pt 1800 mils 600 mils) (pt 1800 mils -900 mils) (width 6 mils))
		(line (pt 1800 mils -900 mils) (pt 200 mils -900 mils) (width 6 mils))
		(line (pt 200 mils -900 mils) (pt 200 mils 600 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1850 mils 800 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "ADXL343BCCZ-RL" (originalName "ADXL343BCCZ-RL") (compHeader (numPins 14) (numParts 1) (refDesPrefix IC)
		)
		(compPin "1" (pinName "VDD I/O") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "GND_1") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "RESERVED_1") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "4" (pinName "GND_2") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "5" (pinName "GND_3") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "6" (pinName "VS") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "7" (pinName "__CS") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "8" (pinName "INT1") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "9" (pinName "INT2") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "10" (pinName "NC") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "11" (pinName "RESERVED_2") (partNum 1) (symPinNum 11) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "12" (pinName "SDO/ALT ADDRESS") (partNum 1) (symPinNum 12) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "13" (pinName "SDA/SDI/SDIO") (partNum 1) (symPinNum 13) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "14" (pinName "SCL/SCLK") (partNum 1) (symPinNum 14) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "ADXL343BCCZ-RL"))
		(attachedPattern (patternNum 1) (patternName "ADXL343BCCZRL")
			(numPads 14)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "9")
				(padNum 10) (compPinRef "10")
				(padNum 11) (compPinRef "11")
				(padNum 12) (compPinRef "12")
				(padNum 13) (compPinRef "13")
				(padNum 14) (compPinRef "14")
			)
		)
		(attr "Manufacturer_Name" "Analog Devices")
		(attr "Manufacturer_Part_Number" "ADXL343BCCZ-RL")
		(attr "Mouser Part Number" "584-ADXL343BCCZ-R")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Analog-Devices/ADXL343BCCZ-RL?qs=WIvQP4zGaniHpowyrbt1Aw%3D%3D")
		(attr "Arrow Part Number" "ADXL343BCCZ-RL")
		(attr "Arrow Price/Stock" "https://www.arrow.com/en/products/adxl343bccz-rl/analog-devices?region=nac")
		(attr "Description" "Accelerometer X, Y, Z Axis +/-2g, 4g, 8g, 16g 0.05Hz ~ 1.6kHz 14-LGA (3x5)")
		(attr "Datasheet Link" "https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL343.pdf")
		(attr "Height" "1 mm")
	)

)
