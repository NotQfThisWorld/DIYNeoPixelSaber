# DIYNeoPixelSaber

![20230824_154503](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/eb88688a-9a80-4d6f-bde4-b064434a8532)


This is the github for a project I made. It is called the NeoPixelSaber.
My goal was to make a lightsaber using an arduino nano, and relativly cheap or even salvaged parts.



## What I used:

### Materials:
- _1x Arduino Nano_
- _1x momentary push-button_
- _1x powerbutton from a Dolce Gusto capsule coffe maker_
- _1x On/Off switch_
- _2x 1m 144LED WS2812B Addressable LED-strip_
- _1x Capacitor 1000 µF from a printer-powersupply_
- _1x Capacitor 2200 µF from a printer-powersupply_
- _2x 330Ω resistor_
- _2x 10 000Ω resistor_
- _A 4pin plug from a printer-powersupply_
- _1x 2x2 AA battery holder_
- _TP cable_
- _Bubblewrap_
- _3x Schotch Tape Rolls_
- _1x Milk Screw Cap_
- _Small 12x2cm acrylic plate I had lying around._
- _4x 1,2V rechargable AA-batteries_
- _1m PP-tube 32mm_
- _1m PP-tube 40mm_
- _1x Sleeve for 32mm tubes_
- _Some random bike parts found in a trench_
- _Screews normally used in pc-cabinets_
- _Hobby popsicle-sticks_
- _Electrical Tape_

### Tools:
- _Soldering Iron_
- _Knife_
- _Circle Saw_
- _Sandpaper_
- _Paint and spray-paint_
- _Drill_
- _Hotglue-gun_



## Suggested Procedure

This is only a brief explanation of how you _can_ do it. This is because I want you to make your own neopixelsaber, not just copy mine. Be carefull, so that you don't damage any materials or equiment. I take no responsibility if you mess up. Make sure you know what you are doing. It is wise to read through the whole thing _before_ beginning.

###   --Hilt--
1. First, you will want to prototype your project. I used a 24 pixel neopixel ring, some buttons and some LEDS. I recomend using an arduino UNO for prototyping, for easy connectivity, however that is only a personal preference.
2. Then, you will want to write the code for it. Although you can go ahead and use my code, I strongly suggest that you try to write the code yourself. That way, you get a better understanding of how your lightsaber works, as well as the knowlegde of how the code works. It will also then be easier to add features to the code if you know how it functions. (+ that my code is terrible).
No need to write the full code now, you can adjust it as you go.
3. Time to design your lightsaber. Make drawings. Go for a walk in a hardware store. Be creative.
4.Buy or otherwise acquire the materials you need. You can of course acquire the materials as you go too.
5. Cut the PP-Tubes up into reasonable lengths. Using the 40mm tubes I went for 25cm for the main hilt, and 5cm for shroud. Then I used a 32mm sleeve, that I sanded down so it would fit inside the hilt and the shroud, to connect the two parts together. This sleeve would also serve as a holder for the blade. The blade (made of the 32mm tube) should be between 0,8M and 1M long.
6. Make the electronics compartment. I used a 2x2 AA batteryholder as my starting point. I attached a plate to the back of the holder, to hold the Arduino and the electronics. I added a couple of empty scotch tape rolls to it to make it so that the electronics compartment would fit nicely inside the hilt. Test it inside the "hilt tube" to see if it fits. Remember, that everything you do from here on, must also be able to fit inside the tube.
7. Plan where you want to place the arduino Nano, what wires you need to solder, what and how many buttons you want, and find out how you will want to go forward.
8. Start soldering. Make sure you have enough length on each wire (better safe than sorry) and that the wires are drawn trough any holes or channels that you want them to go through.
9. Remember to add a main powerswitch. I put mine inside of the bottom cap, but If you can find a better place for it, go ahead.

###   --Blade--
10. Cut the strips to length. Glue the two strips together, back to back. Solder the connections to the plug. Add a 1000 µF capasitor between the + and -.
11. Find a way to make the electronics part of the strip fit snuggly into the bottom of the blade. I used a small sleeve with electrical tape around. Make also sure that the "dark" electronics part of the blade is hidden when you put it into the hilt. Hotglue the end so that only the plug is accessible
12. Find something to put on the other end of the blade. I used a pice of white PE-foam (you often find them protecting electronics like screens). However, this gave me a bit of a flat tip. Try to find something cone-shaped. Also, by sanding down the blade, you can allow more light through, which makes for a brighter blade. But, however, this may lead to a less smooth blade.
13. Your blade is now more or less done.

###   --Hilt--
14. Place the buttons on the electronics compartment. Make sure that they are as close to flush with the inside of the hilt as possible.
15. Find out where the buttons lines up. Drill holes for the buttons.
16. Now make sure that everything fits together. The electronics inside the hilt, the blade inside the top of the hilt/the sleeve.
17. Upload the code. Make sure to update any pin numbers or LED-strip lengths before uploading, in case you have prototyped with another board and/or LED-strip. Adjust the code as you see fit. Colors is very likely to need adjusting, as the white tube dissorts some of the colors quite a bit.
18. Now comes the fun part: Decorating. There is really no right or wrong here. Just make sure that nothing of your decor comes in the way for the electronics or the button-holes. I would start with the paint, before adding other decor later.
19. Your lightsaber is pretty much done. Congrats, young padawan. You have successfully constructed you very own lightsaber.
20. Show it to all of your friends :D.



## Gallery
![20230822_180835](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/0053589a-f2af-4c14-bfaf-bd9e64663454)
![20230822_181004](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/2e862715-6fb2-49dc-9b35-af4becda76f9)
![20230822_181655](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/795b4370-064a-4b11-ab96-1d6bce60db9d)
![20230822_181719](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/4fa35747-e82b-4fe7-90df-0d47fe08c369)
![20230824_160533](https://github.com/NotQfThisWorld/DIYNeoPixelSaber/assets/113789597/0057b305-163c-4c66-8f70-cc37f5f22ef4)

##QnA

### Q: Why no sound?
A: Because of interrupt problems. When I tried to use a buzzer with a steady tone (the humming), the sound would change everytime something happened on the blade. Since it was unreliable (and that I had no way of detecting when the user would swing his blade) I decided to cut the feature. I may add it in the future, maybe with a dedicated arduino to controll the sound effects.

### Q: How much did it cost?
A: A rough estimate is around 1500 Norwegian Kroners, which translates into around 140 dollars.

### Q: How long time did you spend on the project?
A: I started in late December 2022, and finished around 17. May 2023. So, around 4 months.

### Q: What was the hardest part?
A: The hardest part was probably the blade. Getting the strip with the diffuser around it into the blade without making it uneven, was really hard. And doing the electronics part of the blade was hard, since I had to make sure that the electronics part wouldn't show on the outside.

### Q: Will there be a version 2?
A: Only time will tell. I may upgrade the current saber with features (Sound maybe?), but I really want to make a version 2 when I am more experienced and have more resources.

## Contact Me
If you have any questions, need help with anything, or have suggestions, feel free to shoot me a message on Discord: @NotQfThisWorld
