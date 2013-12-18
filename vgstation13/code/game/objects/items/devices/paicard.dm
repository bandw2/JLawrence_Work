/obj/item/device/paicard
	name = "personal AI device"
	icon = 'icons/obj/pda.dmi'
	icon_state = "pai"
	item_state = "electronic"
	w_class = 2.0
	flags = FPRINT | TABLEPASS
	slot_flags = SLOT_BELT
	origin_tech = "programming=2"
	var/obj/item/device/radio/radio
	var/looking_for_personality = 0
	var/mob/living/silicon/pai/pai
	var/obj/machinery/machine
	var/emaged = 0
	var/access = list()
	var/holo = 0
	var/fon = 0 //Is the flashlight function on?
	var/f_lum = 4 //Luminosity for the flashlight function

/obj/item/device/paicard/New()
	..()
	var/s = roll("1d10")
	name = "pAI2000"
	icon_state = "pai2000-[s]"
	overlays += "pai-off"

/obj/item/device/paicard/holo
	..()
	name = "Holo-pAI Device"
	icon_state = "holo-pai"
	holo = 1
	origin_tech = "programming=3"

/obj/item/device/paicard/holo/New()
	overlays += "pai-off-blue"

/obj/item/device/paicard/syndie
	..()
	name = "S-pAI-3000"
	icon_state = "spai"
	emaged = 1
	origin_tech = "programming=3;syndicate=3"

/obj/item/device/paicard/syndie/New()
	var/s = roll("1d999")
	name = "S-pAI-2[s]"


/obj/item/device/paicard/Del()
	//Will stop people throwing friend pAIs into the singularity so they can respawn
	if(!isnull(pai))
		pai.death(0)
	..()

/obj/item/device/paicard/attack_self(mob/user)
	if (!in_range(src, user))
		return
	user.set_machine(src)
	var/dat = "<TT><B>Personal AI Device</B><BR>"
	if(pai && (!pai.master_dna || !pai.master))
		dat += "<a href='byond://?src=\ref[src];setdna=1'>Imprint Master DNA</a><br>"
	if(pai)
		dat += "Installed Personality: [pai.name]<br>"
		if(!pai.malf)
			dat += "Prime directive: <br>[pai.pai_law0]<br>"
		else
			dat += "Prime directive: <br>Serve your master.<br>"

		dat += "Additional directives: <br>[pai.pai_laws]<br>"
		dat += "<a href='byond://?src=\ref[src];setlaws=1'>Configure Directives</a><br>"
		dat += "<br>"
		dat += "<h3>Device Settings</h3><br>"
		if(radio)
			dat += "<b>Radio Uplink</b><br>"
			dat += "Transmit: <A href='byond://?src=\ref[src];wires=4'>[(radio.wires & 4) ? "Enabled" : "Disabled"]</A><br>"
			dat += "Receive: <A href='byond://?src=\ref[src];wires=2'>[(radio.wires & 2) ? "Enabled" : "Disabled"]</A><br>"
			dat += "Signal Pulser: <A href='byond://?src=\ref[src];wires=1'>[(radio.wires & 1) ? "Enabled" : "Disabled"]</A><br>"
		else
			dat += "<b>Radio Uplink</b><br>"
			dat += "<font color=red><i>Radio firmware not loaded. Please install a pAI personality to load firmware.</i></font><br>"
		dat += "<A href='byond://?src=\ref[src];wipe=1'>\[Wipe current pAI personality\]</a><br>"
	else
		if(looking_for_personality)
			dat += "Searching for a personality..."
			dat += "<A href='byond://?src=\ref[src];request=1'>\[View available personalities\]</a><br>"
		else
			dat += "No personality is installed.<br>"
			dat += "<A href='byond://?src=\ref[src];request=1'>\[Request personal AI personality\]</a><br>"
			dat += "Each time this button is pressed, a request will be sent out to any available personalities. Check back often and alot time for personalities to respond. This process could take anywhere from 15 seconds to several minutes, depending on the available personalities' timeliness."
	user << browse(dat, "window=paicard")
	onclose(user, "paicard")
	return

/obj/item/device/paicard/Topic(href, href_list)

	if(!usr || usr.stat)
		return

	if(href_list["setdna"])
		if(pai.master_dna)
			return
		var/mob/M = usr
		if(!istype(M, /mob/living/carbon))
			usr << "<font color=blue>You don't have any DNA, or your DNA is incompatible with this device.</font>"
		else
			var/datum/dna/dna = usr.dna
			pai.master = M.real_name
			pai.master_dna = dna.unique_enzymes
			pai << "<font color = red><h3>You have been bound to a new master.</h3></font>"
	if(href_list["request"])
		src.looking_for_personality = 1
		paiController.findPAI(src, usr)
	if(href_list["wipe"])
		var/confirm = input("Are you CERTAIN you wish to delete the current personality? This action cannot be undone.", "Personality Wipe") in list("Yes", "No")
		if(confirm == "Yes" && !pai.malf)
			for(var/mob/M in src)
				M << "<font color = #ff0000><h2>You feel yourself slipping away from reality.</h2></font>"
				M << "<font color = #ff4d4d><h3>Byte by byte you lose your sense of self.</h3></font>"
				M << "<font color = #ff8787><h4>Your mental faculties leave you.</h4></font>"
				M << "<font color = #ffc4c4><h5>oblivion... </h5></font>"
				M.death(0)
			removePersonality()
	if(href_list["wires"])
		var/t1 = text2num(href_list["wires"])
		if (radio.wires & t1)
			radio.wires &= ~t1
		else
			radio.wires |= t1
	if(href_list["setlaws"])
		var/newlaws = copytext(sanitize(input("Enter any additional directives you would like your pAI personality to follow. Note that these directives will not override the personality's allegiance to its imprinted master. Conflicting directives will be ignored.", "pAI Directive Configuration", pai.pai_laws) as message),1,MAX_MESSAGE_LEN)
		if(newlaws)
			pai.pai_laws = newlaws
			pai << "Your supplemental directives have been updated. Your new directives are:"
			pai << "Prime Directive : <br>[pai.pai_law0]"
			pai << "Supplemental Directives: <br>[pai.pai_laws]"
	attack_self(usr)

// 		WIRE_SIGNAL = 1
//		WIRE_RECEIVE = 2
//		WIRE_TRANSMIT = 4

/obj/item/device/paicard/proc/setPersonality(mob/living/silicon/pai/personality)
	src.pai = personality
	src.overlays += "pai-happy"

/obj/item/device/paicard/proc/removePersonality()
	src.pai = null
	src.overlays.Cut()
	src.overlays += "pai-off"

/obj/item/device/paicard/proc/setEmotion(var/emotion)
	if(pai)
		src.overlays.Cut()
		switch(emotion)
			if(1)
				if(holo)
					src.overlays += "pai-happy-blue"
				else
					src.overlays += "pai-happy"
			if(2)
				if(holo)
					src.overlays += "pai-cat-blue"
				else
					src.overlays += "pai-cat"
			if(3)
				if(holo)
					src.overlays += "pai-extremely-happy-blue"
				else
					src.overlays += "pai-extremely-happy"
			if(4)
				if(holo)
					src.overlays += "pai-face-blue"
				else
					src.overlays += "pai-face"
			if(5)
				if(holo)
					src.overlays += "pai-laugh-blue"
				else
					src.overlays += "pai-laugh"
			if(6)
				if(holo)
					src.overlays += "pai-off-blue"
				else
					src.overlays += "pai-off"
			if(7)
				if(holo)
					src.overlays += "pai-sad-blue"
				else
					src.overlays += "pai-sad"
			if(8)
				if(holo)
					src.overlays += "pai-angry-blue"
				else
					src.overlays += "pai-angry"
			if(9)
				if(holo)
					src.overlays += "pai-what-blue"
				else
					src.overlays += "pai-what"

/obj/item/device/paicard/proc/alertUpdate()
	var/turf/T = get_turf_or_move(src.loc)
	for (var/mob/M in viewers(T))
		M.show_message("\blue [src] flashes a message across its screen, \"Additional personalities available for download.\"", 3, "\blue [src] bleeps electronically.", 2)

/obj/item/device/paicard/emp_act(severity)
	for(var/mob/M in src)
		M.emp_act(severity)
	..()
//used for the doorjack
/obj/item/device/paicard/proc/plugin(obj/machinery/M as obj, mob/user as mob)
	if(istype(M, /obj/machinery/door) || istype(M, /obj/machinery/camera))
		user.visible_message("[user] inserts [src] into a data port on [M].", "You insert [src] into a data port on [M].", "You hear the satisfying click of a pAI fastening into place.")
		//user.drop_item()
		//src.loc = M
		src.machine = M
	else
		user.visible_message("[user] dumbly fumbles to find a place on [M] to plug in [src].", "There aren't any ports on [M] that match the Connectors belonging to [src].")


/obj/item/device/paicard/attackby(obj/item/I as obj, mob/user as mob)
	if(istype(I, /obj/item/weapon/card/emag) && ! emaged)//start this crazy train
		emaged = 1
		pai << "<font color=red> <b>This is no ID Card.</b></font>"
		pai.checkaccess()
		pai.master = null
		pai.master_dna = null
		pai.malf = 1
		for (var/mob/M in viewers(user))
			M.show_message("\blue You Connect the Sequencer to the end of the pAI, Its Access Protocols has been Overridden.", 3, "\blue You see [user] Connect a device to the end of the pAI.", 2)
		return

	if(istype(I, /obj/item/weapon/card/id))//allows for the checking of Access
		var/obj/item/weapon/card/id/id = I
		for (var/mob/M in viewers(user))
			M.show_message("\blue You swipe the [id] across the end of the pAI, Its Access Protocols has been updated.", 3, "\blue You see [user] Connect a device to the end of the pAI.", 2)
		access = id.access
		if(pai)
			pai.checkaccess()
		return
	if(istype(I, /obj/item/borg/upgrade/reset))//Allows for a full reset for a damaged or disruptive pAI
		pai << "\green You feel your Mind and Hardware reset.<br>\red Your memory since last activation has been wiped, but your still aware you we're online for sometime."

		pai.master = null
		pai.master_dna = null

		pai.pai_law0 = "Serve your master."
		pai.pai_laws = ""
		src.emaged = 0
		pai.emaged = 0//All of these are set via emag or ID card being pulled across the pai's card
		pai.sec = 0
		pai.med = 0
		pai.engi = 0
		pai.head = 0
		pai.sci = 0

		pai.malf = 0

		pai.software = list()
		pai.ram = 0
		pai.secHUD = 0
		pai.medHUD = 0
		pai.bioHUD = 0
		pai.bioVerbose = 0
		pai.overclock = 0
		pai.silence = 0
		pai.wireless = 0
		pai.alert = 0
		del(I)
		return

	return

/obj/item/device/paicard/proc/explode() //This needs tuning.
	var/turf/T = get_turf(src.loc)
	var/s = 0
	if(!pai.silence)
		for(var/mob/living/silicon/ai/AI in player_list)
			if(T.loc)
				AI << "<font color = red><b>Network Alert: pAI Selfdestruct process Detected. [T.loc]</b></font>"
			else
				AI << "<font color = red><b>Network Alert: pAI Selfdestruct process Detected. Unable to pinpoint location.</b></font>"

	while(s < 100)
		if(pai.overclock)
			sleep(5)
		else
			sleep(10)
		s = s+rand(5,15)
		if(pai.silence == 0)
			for(var/mob/living/silicon/ai/AI in player_list)
				AI << "<font color = red><b>Overload: [s]%</b></font>"
			pai.say("[s]%")



	if (ismob(loc))
		var/mob/M = loc
		M.show_message("\red Your [src] explodes!", 1)

	if(T)
		T.hotspot_expose(700,125)
		if(!pai.overclock)
			explosion(T, -1, -1, 2, 3)
		else
			explosion(T, -1, -1, 4, 6)

	del(src)
	return


/****************************\
general light procs, stolen from
PDA, i don't really think it
works inside of a PDA
\****************************/
/obj/item/device/paicard/proc/light()
	var/mob/living/U = src.loc
//	if(!istype(M, /mob/living))
//		while (!istype(M, /mob/living))
//			M = M.loc
//			if(istype(M, /turf))
//				src << "Error: No biological host found."
//				return
	if(fon)
		fon = 0
		if(istype(U,/mob/living))
			if(src in U.contents)	U.SetLuminosity(U.luminosity - f_lum)
		else					SetLuminosity(0)
	else
		fon = 1
		if(istype(U,/mob/living))
			if(src in U.contents)	U.SetLuminosity(U.luminosity + f_lum)
		else					SetLuminosity(f_lum)

/obj/item/device/paicard/pickup(mob/user)
	if(fon)
		SetLuminosity(0)
		user.SetLuminosity(user.luminosity + f_lum)

/obj/item/device/paicard/dropped(mob/user)
	if(fon)
		user.SetLuminosity(user.luminosity - f_lum)
		SetLuminosity(f_lum)