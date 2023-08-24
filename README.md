# DIYNeoPixelSaber
This is the github for a project I am working on. It is called the NeoPixelSaber.
My goal was to make a lightsaber using an arduino nano, and relativly cheap parts.

## Part List:
- 1x Arduino Nano
- 2x Momentary Push Button
- 2x Indicator leds (optional, can also be integrated into one of the buttons).
- 1x On/Off switch
- 2x 1m 144LED NeoPixelStip
- Some sort of diffuser. I used bubblewrap, but there are better alternatives.
- 1x Capacitor 1000 µF
- Resistors for both your Strip and you buttons. May vary from product to product.
- 1x Plug with atleast 4 pins.
- 1x 2x2 AA battery holder
- 4x 1,2V rechargable AA-batteries
- 1m PP-tube 32mm
- 1m PP-tube 40mm
- 1x Sleeve
- TP cable
- Various things for decorating. (Be creative)

## Tools:
- Soldering Iron
- Knife
- Saw (circle saw or similar for better cuts)
- Sandpaper
- Paint and/or spray-paint
- Drill

## Suggested Procedure

This is only a brief explination of how I would want to go forward. Be carefull, so that you don't damage any materials or equiment. I take no responsibility for any damaged materials or equipment. Make sure you know what you are doing.

###   --Hilt--
1. First, you will want to prototype your project. I used a 24 pixel neopixel ring, some buttons and some LEDS. I recomend using an arduino UNO for prototyping, for easy connectivity, however that is only a personal preference.
2. Then, you will want to write the code for it. Although you can go ahead and use my code, I strongly suggest that you try to write the code yourself. That way, you get a better understanding of how your lightsaber works, as well as the knowlegde of how the code works. It will also then be easier to add features to the code if you know how it functions. (+ that my code is terrible).
No need to write the full code now, you can adjust it as you go.
3. Time to design your lightsaber. Make drawings. Go for a walk in a hardwars store. Be creative.
4.Buy the materials you need. You can of course buy the materials as you go too.
5. Cut the PP-Tubes up into reasonable lengths. Using the 40mm tubes I went for 25cm for the main hilt, and 5cm for shroud. Then I used a 32mm sleeve, that I sanded down so it would fit inside the hilt and the shroud, to connect the two parts together. This sleeve would also serve as a holder for the blade. The blade (made of the 32mm tube) should be between 0,8M and 1M long.
6. Make the electronics compartment. I used a 2x2 AA batteryholder as my starting point. I attached a plate to the back of the holder, to hold the Arduino and the electronics. I added a couple of empty scotch tape rolls to it to make it so that the electronics compartment would fit nicely inside the hilt. Test it inside the "hilt tube" to see if it fits. Remember, that everything you do from here on, must also be able to fit inside the tube.
7. Plan where you want to place the arduino Nano, what wires you need to solder, what and how many buttons you want, and find out how you will want to go forward.
8. Start soldering. Make sure you have enough length on each wire (better safe than sorry) and that the wires are drawn trough any holes or channels that you want them to go through.
9. Remember to add a main powerswitch. I put mine inside of the bottom cap, but If you can find a better place for it, go ahead.

###   --Blade--
10. Cut the strips to length. Glue the two strips together, back to back. Solder the connections to the plug. Add a 1000 µF capasitor between the + and -.
11. Find a way to make the electronics part of the strip fit snuggly into the bottom of the blade. I used a small sleeve with electrical tape around. Make also sure that the "dark" electronics part of the blade is hidden when you put it into the hilt. Hotglue the end so that only the plug is accessible
12. Find something to put on the end of the blade. I used a pice of white PE-foam (you often find them protecting electronics like screens). However, this gave me a bit of a flat tip. Try to find something cone-shaped. Also, by sanding down the blade, you can allow more light through, which makes for a brighter blade. But, however, this may lead to a less smooth blade.
13. Your blade is now more or less done.

###   --Hilt--
14. Place the buttons on the electronics compartment. Make sure that they are as close to flush with the inside of the hilt as possible.
15. Find out where the buttons lines up. Drill holes for the buttons.
16. Now make sure that everything fits together. The electronics inside the hilt, the blade inside the top of the hilt/the sleeve.
17. Upload the code. Make sure to update any pins or led-strip lengths before uploading, in case you have prototyped with another board and/or led-strip. Adjust the code as you see fit. Colors is very likely to need adjusting, as the white tube dissorts some of the colors quite a bit.
18. Now comes the fun part: Decorating. There is really no right or wrong here. Just make sure that nothing of your decor comes in the way for the electronics or the button-holes. I would start with the paint, before adding other decor later.
19. Your lightsaber is pretty much done. Congrats, young padawan. You have successfully constructed you very own lightsaber.
