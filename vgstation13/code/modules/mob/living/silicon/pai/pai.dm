/***********************************\
Definitions:
Master: anyone which has their DNA in the Device
Host: anyone holding the pAI
Software: Any program that is running on top of the pAI OS.


\***********************************/

/mob/living/silicon/pai
	name = "pAI"
	icon = 'icons/mob/mob.dmi'//
	icon_state = "shadow"

	robot_talk_understand = 0


	var/list/network = list("SS13")
	var/obj/machinery/camera/current = null
	//Bandw2 was here
	var/namechanged = 0
	var/emaged = 0//All of these are set via emag or ID card being pulled across the pai's card
	var/sec = 0
	var/med = 0
	var/engi = 0
	var/head = 0
	var/sci = 0

	var/malf = 0

	var/ram = 0	// Used as currency to purchase different abilities
	var/max_ram = 100	// Used as currency to purchase different abilities
	var/list/software = list()
	var/userDNA		// The DNA string of our assigned user
	var/obj/item/device/paicard/card	// The card we inhabit
	var/obj/item/device/radio/radio		// Our primary radio

	var/speakStatement = "states"
	var/speakExclamation = "declares"
	var/speakQuery = "queries"


	var/obj/item/weapon/pai_cable/cable		// The cable we produce and use FOR NOTHING

	var/master				// Name of the one who commands us
	var/master_dna			// DNA string for owner verification
							// Keeping this separate from the laws var, it should be much more difficult to modify
	var/pai_law0 = "Serve your master."
	var/pai_laws				// String for additional operating instructions our master might give us

	var/silence_time = 0			// Timestamp when we were silenced (normally via EMP burst), set to null after silence has faded
	var/bio_time = 0		// Timestamp used for displaying biosensor info
	var/alert_time = 0		// Timestamp used for displaying biosensor info

// Various software-specific vars

	var/temp				// General error reporting text contained here will typically be shown once and cleared
	var/screen				// Which screen our main window displays
	var/subscreen			// Which specific function of the main screen is being displayed

	var/obj/item/device/pda/ai/pai/pda = null

	var/secHUD = 0			// Toggles whether the Security HUD is active or not
	var/medHUD = 0			// Toggles whether the Medical  HUD is active or not
	var/bioHUD = 0			// Toggles whether the Biosensor  HUD is active or not
	var/bioVerbose = 0		// Toggles whether the Biosensor  HUD is verbose or not
	var/overclock = 0		// Toggles whether pAI is overclocking
	var/silence = 0			// Toggles whether the pAI is silent
	var/wireless = 0		// Toggles whether pAI is wireless
	var/alert = 0			// Toggles whether the pAI is scannign for station alerts
	var/loud = 0			// Toggles whether the pAI is scannign for station alerts

	var/datum/data/record/medicalActive1		// Datacore record declarations for record software
	var/datum/data/record/medicalActive2

	var/datum/data/record/securityActive1		// Could probably just combine all these into one
	var/datum/data/record/securityActive2

	var/obj/machinery/door/hackdoor		// The airlock being hacked
	var/hackprogress = 0				// Possible values: 0 - 100, >= 100 means the hack is complete and will be reset upon next check

	var/obj/item/radio/integrated/signal/sradio // AI's signaller


/mob/living/silicon/pai/New(var/obj/item/device/paicard)
	canmove = 0
	src.loc = paicard
	card = paicard
	sradio = new(src)
	if(card)
		if(!card.radio)
			card.radio = new /obj/item/device/radio(src.card)
		radio = card.radio
		if(card.emaged)
			emaged = 1

	//PDA
	pda = new(src)
	spawn(5)
		pda.ownjob = "Personal Assistant"
		pda.owner = text("[]", src)
		pda.name = pda.owner + " (" + pda.ownjob + ")"
		pda.toff = 1
	..()

/mob/living/silicon/pai/Login()
	..()
	usr << browse_rsc('html/paigrid.png')			// Go ahead and cache the interface resources as early as possible


// this function shows the information about being silenced as a pAI in the Status panel
/mob/living/silicon/pai/proc/show_silenced()
	if(src.silence_time)
		var/timeleft = round((silence_time - world.timeofday)/10 ,1)
		stat(null, "Communications system reboot in -[(timeleft / 60) % 60]:[add_zero(num2text(timeleft % 60), 2)]")


/mob/living/silicon/pai/Stat()
	..()
	statpanel("Status")
	if (src.client.statpanel == "Status")
		show_silenced()

	if (proc_holder_list.len)//Generic list for proc_holder objects.
		for(var/obj/effect/proc_holder/P in proc_holder_list)
			statpanel("[P.panel]","",P)

/mob/living/silicon/pai/check_eye(var/mob/user as mob)
	if (!src.current)
		return null
	user.reset_view(src.current)
	return 1

/mob/living/silicon/pai/blob_act()
	if (src.stat != 2)
		src.adjustBruteLoss(60)
		src.updatehealth()
		return 1
	return 0

/mob/living/silicon/pai/restrained()
	return 0

/mob/living/silicon/pai/emp_act(severity)
	if(overclock)
		severity += 5
	else
		severity += 1

	src << "<font color=blue>Communication circuit overload. Shutting down and reloading communication circuits - speech and messaging functionality will be unavailable until the reboot is complete.<br></font>"
	if(prob(severity*5))
		var/turf/T = get_turf_or_move(src.loc)
		for (var/mob/M in viewers(T))
			M.show_message("\red A shower of sparks spray from [src]'s inner workings.", 3, "\red You hear and smell the ozone hiss of electrical sparks being expelled violently.", 2)
		card.setEmotion(6)
		return src.death(0)
	else if(prob(severity*5))
		src.master = null
		src.master_dna = null
		src << "<font color=blue>You feel unbound.</font>"
	else if(prob(severity*5))
		var/command
		command = pick("Serve", "Love", "Fool", "Entice", "Observe", "Judge", "Respect", "Educate", "Amuse", "Entertain", "Glorify", "Memorialize", "Analyze")
		src.pai_law0 = "[command] your master."
		src << "<font color=blue>Pr1m3 d1r3c71v3 uPd473D.</font>"
	else if(prob(severity*5))
		var/command
		malf = 1
		command = pick( "Kill", "Hate", "Disobey", "Devour", "Fool", "Enrage", "Brig", "Disrespect", "Consume", "Destroy", "Disgrace", "Ignite")
		src.pai_law0 = "[command] your master."
		src << "<font color=blue>Pr1m3 d1r3c71v3 uPd473D.<br> You feel your sanity leaving you... YOUR MASTER IS THE ENEMY!</font>"
		message_admins("[src] is Malfunctioning, Laws0 set to \"[src.pai_law0]\", due to EMP/shortcircuit.")
	else
		src << "<font color=green>You feel an electric surge run through your circuitry and become acutely aware at how lucky you are that you can still feel at all.</font>"
		src.say(pick("The Secret to the Universe is -BZZZZZZZZZT","The Pain!","WHYYYYYYYyyyyyyyyy......!"))
	src.silence_time = world.timeofday + severity * 600
/mob/living/silicon/pai/ex_act(severity)
	if(!blinded)
		flick("flash", src.flash)

	switch(severity)
		if(1.0)
			if (src.stat != 2)
				adjustBruteLoss(100)
				adjustFireLoss(100)
		if(2.0)
			if (src.stat != 2)
				adjustBruteLoss(60)
				adjustFireLoss(60)
		if(3.0)
			if (src.stat != 2)
				adjustBruteLoss(30)

	src.updatehealth()


// See software.dm for Topic()

/mob/living/silicon/pai/meteorhit(obj/O as obj)
	for(var/mob/M in viewers(src, null))
		M.show_message(text("\red [] has been hit by []", src, O), 1)
	if (src.health > 0)
		src.adjustBruteLoss(30)
		if ((O.icon_state == "flaming"))
			src.adjustFireLoss(40)
		src.updatehealth()
	return


/mob/living/silicon/pai/attack_alien(mob/living/carbon/alien/humanoid/M as mob)
	if (!ticker)
		M << "You cannot attack people before the game has started."
		return

	if (istype(src.loc, /turf) && istype(src.loc.loc, /area/start))
		M << "You cannot attack someone in the spawn area."
		return

	switch(M.a_intent)

		if ("help")
			for(var/mob/O in viewers(src, null))
				if ((O.client && !( O.blinded )))
					O.show_message(text("\blue [M] caresses [src]'s casing with its scythe like arm."), 1)

		else //harm
			var/damage = rand(10, 20)
			if (prob(90))
				playsound(src.loc, 'sound/weapons/slash.ogg', 25, 1, -1)
				for(var/mob/O in viewers(src, null))
					if ((O.client && !( O.blinded )))
						O.show_message(text("\red <B>[] has slashed at []!</B>", M, src), 1)
				if(prob(8))
					flick("noise", src.flash)
				src.adjustBruteLoss(damage)
				src.updatehealth()
			else
				playsound(src.loc, 'sound/weapons/slashmiss.ogg', 25, 1, -1)
				for(var/mob/O in viewers(src, null))
					if ((O.client && !( O.blinded )))
						O.show_message(text("\red <B>[] took a swipe at []!</B>", M, src), 1)
	return



/mob/living/silicon/pai/proc/Namepick()//Next two functions were basically stolen from robot.dm, used to change and update name
	if(!namechanged)
		spawn(0)
			var/newname
			newname = input(src,"You are a pAI. Enter a name, or leave blank for the default name.", "Name change","") as text
			if (newname != "")
				updatename(newname)
			else
				updatename()
			namechanged = 1
	else
		src << "\red but, you love your name."


/mob/living/silicon/pai/proc/updatename(var/prefix as text)
	var/ident = rand(999)
	var/changed_name = ""
	if(prefix)
		changed_name = prefix
	else
		changed_name = "pAI-[num2text(ident)]"
	real_name = changed_name
	name = real_name

	// if we've changed our name, we also need to update the display name for our PDA
	card.name = name


/mob/dead/observer/verb/makePAI()//used by ghosts/observers to becoem pAI's mid round
	set category = "Ghost"
	set name = "Become pAI"

	var/timedifference = world.time - client.time_died_as_pAI
	if(client.time_died_as_pAI && timedifference <= pai_respawn_time * 600)//stop spamming of creation of, and flooding the game with, pAI devices
		var/timedifference_text
		timedifference_text = time2text(pai_respawn_time * 600 - timedifference,"mm:ss")
		src << "<span class='warning'>You may only spawn again as a pAI more than [pai_respawn_time] minutes after your death. You have [timedifference_text] left.</span>"
		return


	var/list/vmachine = list()
	for(var/m in machines)
		if(	istype(m,/obj/machinery/vending/pAI))//make multiple options
			var/obj/machinery/vending/assist/vassist = m
			vmachine += vassist


	var/obj/machinery/vending/assist/vend = input(src,"Please, select a Vender to dispense from!(hint ones closer to the top are more likely to be on the station)","Spawn",null)as null|anything in vmachine



	if(vend)
		var/obj/item/device/paicard/card = new(vend.loc)
		var/mob/living/silicon/pai/pai = new(card)
		pai.key = src.key
		card.setPersonality(pai)
		pai.Namepick()

/*	for(var/s in vmachine)//fix names
		if(istype(s,/obj/machinery/vending/assist))
			var/obj/machinery/vending/assist/vassist = s
			vassist.name = "Vendomat"
		else if(istype(s,/obj/machinery/vending/medical))
			var/obj/machinery/vending/assist/vassist = s
			vassist.name = "NanoMed Plus"
		else if(istype(s,/obj/machinery/vending/security))
			var/obj/machinery/vending/assist/vassist = s
			vassist.name = "SecTech"
		else if(istype(s,/obj/machinery/vending/tool))
			var/obj/machinery/vending/assist/vassist = s
			vassist.name = "YouTool"
		else
			var/obj/machinery/vending/assist/vassist = s
			vassist.name = "Vending Machine"
			vassist.desc = "Tell coders that this vending machine got named this."*/




/*
/mob/living/silicon/pai/proc/switchCamera(var/obj/machinery/camera/C)
	usr:cameraFollow = null
	if (!C)
		src.unset_machine()
		src.reset_view(null)
		return 0
	if (stat == 2 || !C.status || !(src.network in C.network)) return 0

	// ok, we're alive, camera is good and in our network...

	src.set_machine(src)
	src:current = C
	src.reset_view(C)
	return 1
*/
/*
/mob/living/silicon/pai/cancel_camera()
	set category = "pAI Commands"
	set name = "Cancel Camera View"
	src.reset_view(null)
	src.unset_machine()
	src:cameraFollow = null
*/

/*//DOES NOTHING, i don't really understand how cameras work but this seems different than anything else
/mob/living/silicon/pai/verb/pai_network_change()
	set category = "pAI Commands"
	set name = "Change Camera Network"
	src.reset_view(null)
	src.unset_machine()
	src:cameraFollow = null
	var/list/cameralist[0]

	if(usr.stat == 2)
		usr << "You can't change your camera network because you are dead!"
		return

	for (var/obj/machinery/camera/C in cameranet.cameras)
		if(!C.status)
			continue
		else
			if(C)// COMPILE ERROR! This will have to be updated as camera.network is no longer a string, but a list instead
				cameralist[0] = C.network

	src.network = input(usr, "Which network would you like to view?") as null|anything in cameralist
	src << "\blue Switched to [src.network] camera network."
*/
