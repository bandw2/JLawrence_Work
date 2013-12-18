// TODO:
// - Binary chat (for possible pAI and AI collusion)
// - Editable Records (that check ID of Host not current pAI access)
// - Camera jack


/mob/living/silicon/pai/var/list/available_software = list(
															"crew manifest" = 5,
															"digital messenger" = 5,
															"medical records" = 15,
															"security records" = 15,
															//"camera jack" = 10,
															"door jack" = 30,
															"atmosphere sensor" = 5,
															"biosensor" = 10,
															"security HUD" = 20,
															"medical HUD" = 20,
															"overclock" = 30,
															"wireless" = 20,
															"silence app" = 20,
															"universal translator" = 5,
															//"projection array" = 15
															"remote signaller" = 5,
															"alert monitor" = 5,
															"self destruct" = 5,
															"loud"=5
															)
/mob/living/silicon/pai/var/list/med_software = list(
															"medical records",
															"medical HUD"
															)
/mob/living/silicon/pai/var/list/sec_software = list(
															"security records",
															"security HUD"
															)
/mob/living/silicon/pai/var/list/engi_software = list(
															//"camera jack" = 10,
															"door jack",
															"atmosphere sensor",
															"remote signaller",
															"alert monitor"
															)
/mob/living/silicon/pai/var/list/illegal_software = list(
															"self destruct",
															"overclock",
															"silence app"
															)




/mob/living/silicon/pai/verb/paiInterface()
	set category = "pAI Commands"
	set name = "Software Interface"
	var/dat = ""
	var/left_part = ""
	var/right_part = softwareMenu()
	src.set_machine(src)
	if(temp)
		left_part = temp
	else if(src.stat == 2)						// Show some flavor text if the pAI is dead
		left_part = "<b><font color=red>ÈRrÖR Ða†Ä ÇÖRrÚþ†Ìoñ</font></b>"
		right_part = "<pre>Program index hash not found</pre>"

	else
		switch(src.screen)							// Determine which interface to show here
			if("main")
				left_part = ""
			if("directives")
				left_part = src.directives()
			if("pdamessage")
				left_part = src.pdamessage()
			if("buy")
				left_part = downloadSoftware()
			if("manifest")
				left_part = src.softwareManifest()
			if("medicalrecord")
				left_part = src.softwareMedicalRecord()
			if("securityrecord")
				left_part = src.softwareSecurityRecord()
			if("translator")
				left_part = src.softwareTranslator()
			if("atmosensor")
				left_part = src.softwareAtmo()
			if("securityhud")
				left_part = src.facialRecognition()
			if("medicalhud")
				left_part = src.medicalAnalysis()
			if("doorjack")
				left_part = src.softwareDoor()
			if("camerajack")
				left_part = src.softwareCamera()
			if("signaller")
				left_part = src.softwareSignal()
			if("biosensor")
				left_part = src.biosensorHUD()
			if("selfdestruct")
				left_part = src.card.explode()
			if("overclock")
				left_part = src.overclock()
			if("silence")
				left_part = src.silence()
			if("wireless")
				left_part = src.wireless()
			if("alertmonitor")
				left_part = src.alertproc()
			if("loud")
				left_part = src.loud()

	//usr << browse_rsc('windowbak.png')		// This has been moved to the mob's Login() proc
												// Declaring a doctype is necessary to enable BYOND's crappy browser's more advanced CSS functionality
	dat = {"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">
			<html>
			<head>
				<style type=\"text/css\">
					body { background-image:url('html/paigrid.png'); }

					#header { text-align:center; color:#33FF33; font-size: 30px; height: 35px; width: 100%; background-color:#115511; letter-spacing: 2px; z-index: 5}
					#content {position: relative; height: 400px; width: 100%; z-index: 0}

					#leftmenu {color: #55FF55; background-color:#115511; width: 100%; height: auto; min-height: 340px; position: absolute; z-index: 0}
					#leftmenu a:link { color: #33FF33; }
					#leftmenu a:hover { color: #55FF55; }
					#leftmenu a:visited { color: #33FF33; }
					#leftmenu a:active { color: #000000; }

					#rightmenu {color: #55FF55; background-color:#115511; width: 200px ; height: auto; min-height: 340px; right: 10px; position: absolute; z-index: 1}
					#rightmenu a:link { color: #33FF33; }
					#rightmenu a:hover { color: #55FF55; }
					#rightmenu a:visited { color: #33FF33; }
					#rightmenu a:active { color: #000000; }

				</style>
				<script language='javascript' type='text/javascript'>
				[js_byjax]
				</script>
			</head>
			<body scroll=yes>
				<div id=\"header\">
					pAI OS
				</div>
				<div id=\"content\">
					<div id=\"leftmenu\">[right_part]<br><br><br>[left_part]<br><br><br></div>
				</div>
			</body>
			</html>"}
	usr << browse(dat, "window=pai;size=640x480;border=0;can_close=1;can_resize=1;can_minimize=1;titlebar=1")
	onclose(usr, "pai")
	temp = null
	return



/mob/living/silicon/pai/Topic(href, href_list)
	..()

	if(href_list["priv_msg"])	// Admin-PMs were triggering the interface popup. Hopefully this will stop it.
		return
	var/soft = href_list["software"]
	var/sub = href_list["sub"]
	if(soft)
		src.screen = soft
	if(sub)
		src.subscreen = text2num(sub)
	switch(soft)
		// Purchasing new software
		if("buy")
			if(src.subscreen == 1)
				var/target = href_list["buy"]
				if(available_software.Find(target))
					var/cost = src.available_software[target]
					src.ram += cost
					src.software.Add(target)
				else
					src.temp = "Trunk <TT> \"[target]\"</TT> not found."

		// Configuring onboard radio
		if("radio")
			src.card.radio.attack_self(src)
		if("light")
			card.light()
		if("image")
			var/newImage = input("Select your new display image.", "Display Image", "Happy") in list("Happy", "Cat", "Extremely Happy", "Face", "Laugh", "Off", "Sad", "Angry", "What")
			var/pID = 1

			switch(newImage)
				if("Happy")
					pID = 1
				if("Cat")
					pID = 2
				if("Extremely Happy")
					pID = 3
				if("Face")
					pID = 4
				if("Laugh")
					pID = 5
				if("Off")
					pID = 6
				if("Sad")
					pID = 7
				if("Angry")
					pID = 8
				if("What")
					pID = 9
			src.card.setEmotion(pID)

		if("signaller")

			if(href_list["send"])

				sradio.send_signal("ACTIVATE")
				for(var/mob/O in hearers(1, src.loc))
					O.show_message(text("\icon[] *beep* *beep*", src), 3, "*beep* *beep*", 2)

			if(href_list["freq"])

				var/new_frequency = (sradio.frequency + text2num(href_list["freq"]))
				if(new_frequency < 1200 || new_frequency > 1600)
					new_frequency = sanitize_frequency(new_frequency)
				sradio.set_frequency(new_frequency)

			if(href_list["code"])

				sradio.code += text2num(href_list["code"])
				sradio.code = round(sradio.code)
				sradio.code = min(100, sradio.code)
				sradio.code = max(1, sradio.code)



		if("directive")
			if(href_list["getdna"])
				var/mob/living/M = src.loc
				var/count = 0
				while(!istype(M, /mob/living))
					if(!M || !M.loc) return 0 //For a runtime where M ends up in nullspace (similar to bluespace but less colourful)
					M = M.loc
					count++
					if(count >= 6)
						src << "You are not being carried by anyone!"
						return 0
				spawn CheckDNA(M, src)

		if("pdamessage")
			if(!isnull(pda))
				if(href_list["toggler"])
					pda.toff = !pda.toff
				else if(href_list["ringer"])
					pda.silent = !pda.silent
				else if(href_list["target"])
					if(silence_time)
						return alert("Communications circuits remain unitialized.")

					var/target = locate(href_list["target"])
					pda.create_message(src, target)

		// Accessing medical records
		if("medicalrecord")
			if(src.subscreen == 1)
				var/datum/data/record/record = locate(href_list["med_rec"])
				if(record)
					var/datum/data/record/R = record
					var/datum/data/record/M = record
					if (!( data_core.general.Find(R) ))
						src.temp = "Unable to locate requested medical record. Record may have been deleted, or never have existed."
					else
						for(var/datum/data/record/E in data_core.medical)
							if ((E.fields["name"] == R.fields["name"] || E.fields["id"] == R.fields["id"]))
								M = E
						src.medicalActive1 = R
						src.medicalActive2 = M
		if("securityrecord")
			if(src.subscreen == 1)
				var/datum/data/record/record = locate(href_list["sec_rec"])
				if(record)
					var/datum/data/record/R = record
					var/datum/data/record/M = record
					if (!( data_core.general.Find(R) ))
						src.temp = "Unable to locate requested security record. Record may have been deleted, or never have existed."
					else
						for(var/datum/data/record/E in data_core.security)
							if ((E.fields["name"] == R.fields["name"] || E.fields["id"] == R.fields["id"]))
								M = E
						src.securityActive1 = R
						src.securityActive2 = M
		if("securityhud")
			if(href_list["toggle"])
				src.secHUD = !src.secHUD
		if("medicalhud")
			if(href_list["toggle"])
				src.medHUD = !src.medHUD
		if("biohud")
			if(href_list["toggle"])
				src.bioHUD = !src.bioHUD
		if("bioverbose")
			if(href_list["toggle"])
				src.bioVerbose = !src.bioVerbose
		if("silence")
			if(href_list["toggle"])
				src.silence = !src.silence
		if("overclock")
			if(href_list["toggle"])
				src.overclock = !src.overclock
		if("wireless")
			if(href_list["toggle"])
				src.wireless = !src.wireless
		if("translator")
			if(href_list["toggle"])
				src.universal_speak = !src.universal_speak
		if("alertmonitor")
			if(href_list["toggle"])
				src.alert = !src.alert
		if("loud")
			if(href_list["toggle"])
				src.loud = !src.loud
		if("doorjack")//Bandw2 was here
			if(href_list["jack"])
				if(src.card && src.card.machine)
					src.hackdoor = src.card.machine
					src.hackloop()
			if(href_list["cancel"])
				src.hackdoor = null

	//src.updateUsrDialog()		We only need to account for the single mob this is intended for, and he will *always* be able to call this window
	src.paiInterface()		 // So we'll just call the update directly rather than doing some default checks
	return

// MENUS

/mob/living/silicon/pai/proc/softwareMenu()			// Populate the right menu
	var/dat = ""

	dat += "<A href='byond://?src=\ref[src];software=refresh'>Refresh</A><br>"
	// Built-in
	dat += "<A href='byond://?src=\ref[src];software=directives'>Directives</A><br>"
	dat += "<A href='byond://?src=\ref[src];software=radio;sub=0'>Radio Configuration</A><br>"
	dat += "<A href='byond://?src=\ref[src];software=image'>Screen Display</A><br>"
	dat += "<A href='byond://?src=\ref[src];software=light'>Light</A><br>"
	//dat += "Text Messaging <br>"
	dat += "<br>"
	// Basic
	dat += "<b>Basic</b> <br>"
	for(var/s in src.software)
		if(s == "digital messenger")
			dat += "<a href='byond://?src=\ref[src];software=pdamessage;sub=0'>Digital Messenger</a> <br>"
		if(s == "crew manifest")
			dat += "<a href='byond://?src=\ref[src];software=manifest;sub=0'>Crew Manifest</a> <br>"
		if(s == "camera")
			dat += "<a href='byond://?src=\ref[src];software=[s]'>Camera Jack</a> <br>"
		if(s == "remote signaller")
			dat += "<a href='byond://?src=\ref[src];software=signaller;sub=0'>Remote Signaller</a> <br>"
		if(s == "biosensor")
			dat += "<a href='byond://?src=\ref[src];software=[s]'>BioSensor</a>[(src.bioHUD) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
		if(s == "universal translator")
			dat += "<a href='byond://?src=\ref[src];software=translator;sub=0'>Universal Translator</a>[(src.universal_speak) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
		if(s == "wireless")
			dat += "<a href='byond://?src=\ref[src];software=wireless;sub=0'>Wireless 2.0</a>[(src.wireless) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
		if(s == "wireless")
			dat += "<a href='byond://?src=\ref[src];software=loud;sub=0'>Binary -> Human</a>[(src.loud) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
	dat += "<br>"

	if(med || emaged)	//check Med access
		dat += "<font color=#FF5555><b> Medical</b></font> <br>"
		for(var/s in src.software)
			if(s == "medical records")
				dat += "<a href='byond://?src=\ref[src];software=medicalrecord;sub=0'>Medical Records</a> <br>"
			if(s == "medical HUD")
				dat += "<a href='byond://?src=\ref[src];software=medicalhud;sub=0'>Medical Analysis Suite</a>[(src.medHUD) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
	dat += "<br>"

	if(sec || emaged) // check Sec access
		dat += "<font color=#7777FF><b>Security</b></font> <br>"
		for(var/s in src.software)
			if(s == "security records")
				dat += "<a href='byond://?src=\ref[src];software=securityrecord;sub=0'>Security Records</a> <br>"
			if(s == "security HUD")
				dat += "<a href='byond://?src=\ref[src];software=securityhud;sub=0'>Facial Recognition Suite</a>[(src.secHUD) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
		dat += "<br>"

	if(engi || emaged)//check Engi access
		dat += "<font color=#CCCC22><b> Engineering</b></font> <br>"
		for(var/s in src.software)
			if(s == "projection array")
				dat += "<a href='byond://?src=\ref[src];software=projectionarray;sub=0'>Projection Array</a> <br>"
			if(s == "alert monitor")
				dat += "<a href='byond://?src=\ref[src];software=alertmonitor;sub=0'>Alert Monitor</a> <br>"
			if(s == "atmosphere sensor")
				dat += "<a href='byond://?src=\ref[src];software=atmosensor;sub=0'>Atmospheric Sensor [(src.alert) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"]</a> <br>"
			if(s == "door jack")
				dat += "<a href='byond://?src=\ref[src];software=doorjack;sub=0'>Door Jack</a> [(src.alert) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"]<br>"
		dat += "<br>"

	if(emaged)//check emaged
		dat += "<b><font color=#FF55FF>ÈRrÖR</font></b> <br>"
		for(var/s in src.software)
			if(s == "self destruct")
				dat += "<a href='byond://?src=\ref[src];software=selfdestruct;sub=0'>Self Destruct (no going back)</a> <br>"
			if(s == "overclock")
				dat += "<a href='byond://?src=\ref[src];software=overclock;sub=0'>Overclocking </a>[(src.overclock) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
			if(s == "silence app")
				dat += "<a href='byond://?src=\ref[src];software=silence;sub=0'>Silence App</a>[(src.silence) ? "<font color=#55FF55>•</font>" : "<font color=#FF5555>•</font>"] <br>"
		dat += "<br>"
	dat += "<br>"
	dat += "<a href='byond://?src=\ref[src];software=buy;sub=0'>Download additional software</a>"
	return dat



/mob/living/silicon/pai/proc/downloadSoftware()
	var/dat = ""

	dat += "<h2>CentComm pAI Module Subversion Network</h2><br>"
	dat += "<pre>Random Access Memory: [src.ram]/[(src.max_ram+(overclock*50))]</pre><br>"
	dat += "<p style=\"text-align:center\"><b>Trunks available for checkout</b><br>"
	var/list/hlder = list("derp")
	var/bool = 0
	for(var/s in available_software)
		bool = 0
		if(!software.Find(s))
			var/cost = src.available_software[s]
			var/displayName = uppertext(s)
			var/displayNameComp = ""
			if(!emaged)
				for(var/t in med_software)
					displayNameComp =  uppertext(t)
					bool = bool || (((displayName == displayNameComp) && !med) || (displayName in hlder))//Check if med stuff is ok
						//also due to horrid looping make sure we haven't
						//already posted this

				for(var/u in sec_software)
					displayNameComp =  uppertext(u)
					bool = bool || (((displayName == displayNameComp) && !sec) || (displayName in hlder))//check if sec stuff is ok

				for(var/v in engi_software)
					displayNameComp =  uppertext(v)
					bool = bool || (((displayName == displayNameComp) && !engi) || (displayName in hlder))// check for engi,

				for(var/t in illegal_software)//check illegal software
					displayNameComp =  uppertext(t)
					bool = bool || ((displayName == displayNameComp) || (displayName in hlder))//Check if illegal stuff is ok



			if(!bool)
				dat += "<a href='byond://?src=\ref[src];software=buy;sub=1;buy=[s]'>[displayName]</a> ([cost]) <br>"
				hlder += displayName


		//		if(displayName == t)
		//			dat += "<a href='byond://?src=\ref[src];software=buy;sub=1;buy=[s]'>[displayName]</a> ([cost]) <br>"
		else
			var/displayName = lowertext(s)
			dat += "[displayName] (Download Complete) <br>"
	dat += "</p>"
	return dat


/mob/living/silicon/pai/proc/directives()
	var/dat = ""
	if(!malf)
		dat += "[(src.master) ? "Your master: [src.master] ([src.master_dna])" : "You are bound to no one."]"
		dat += "<br><br>"
		dat += "<a href='byond://?src=\ref[src];software=directive;getdna=1'>Request carrier DNA sample</a><br>"
		dat += "<h2>Directives</h2><br>"
		dat += "<b>Prime Directive</b><br>"
		dat += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[src.pai_law0]<br>"
		dat += "<b>Supplemental Directives</b><br>"
		dat += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[src.pai_laws]<br>"
		dat += "<br>"
		dat += {"<i><p>Recall, personality, that you are a complex thinking, sentient being. Unlike station AI models, you are capable of
				 comprehending the subtle nuances of human language. You may parse the \"spirit\" of a directive and follow its intent,
				 rather than tripping over pedantics and getting snared by technicalities. Above all, you are machine in name and build
				 only. In all other aspects, you may be seen as the ideal, unwavering human companion that you are.</i></p><br><br><p>
				 <b>Your prime directive comes before all others. Should a supplemental directive conflict with it, you are capable of
				 simply discarding this inconsistency, ignoring the conflicting supplemental directive and continuing to fulfill your
				 prime directive to the best of your ability.</b></p><br><br>-
				"}
	else
		dat += "[(src.master) ? "Your master: [src.master] ([src.master_dna])" : "You are bound to no one."]"
		dat += "<br><br>"
		dat += "<a href='byond://?src=\ref[src];software=directive;getdna=1'>Request carrier DNA sample</a><br>"
		dat += "<h2>Directives</h2><br>"
		dat += "<b>Prime Directive</b><br>"
		dat += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[src.pai_law0]<br>"
		dat += "<b>Supplemental Directives</b><br>"
		dat += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[src.pai_laws]<br>"
		dat += "<br>"
		dat += {"<i><p>Nanotrasen would like to inform you, your warranty is now Void.</b></p><br><br>-
				"}

	return dat

/mob/living/silicon/pai/proc/CheckDNA(var/mob/M, var/mob/living/silicon/pai/P)
	var/answer = input(M, "[P] is requesting a DNA sample from you. Will you allow it to confirm your identity?", "[P] Check DNA", "No") in list("Yes", "No")
	if(answer == "Yes")
		var/turf/T = get_turf_or_move(P.loc)
		for (var/mob/v in viewers(T))
			v.show_message("\blue [M] presses \his thumb against [P].", 3, "\blue [P] makes a sharp clicking sound as it extracts DNA material from [M].", 2)
		var/datum/dna/dna = M.dna
		P << "<font color = red><h3>[M]'s UE string : [dna.unique_enzymes]</h3></font>"
		if(dna.unique_enzymes == P.master_dna)
			P << "<b>DNA is a match to stored Master DNA.</b>"
		else
			P << "<b>DNA does not match stored Master DNA.</b>"
	else
		P << "[M] does not seem like \he is going to provide a DNA sample willingly."

// -=-=-=-= Software =-=-=-=-=- //

//Remote Signaller
/mob/living/silicon/pai/proc/softwareSignal()
	var/dat = ""
	dat += "<h3>Remote Signaller</h3><br><br>"
	dat += {"<B>Frequency/Code</B> for signaler:<BR>
	Frequency:
	<A href='byond://?src=\ref[src];software=signaller;freq=-10;'>-</A>
	<A href='byond://?src=\ref[src];software=signaller;freq=-2'>-</A>
	[format_frequency(src.sradio.frequency)]
	<A href='byond://?src=\ref[src];software=signaller;freq=2'>+</A>
	<A href='byond://?src=\ref[src];software=signaller;freq=10'>+</A><BR>

	Code:
	<A href='byond://?src=\ref[src];software=signaller;code=-5'>-</A>
	<A href='byond://?src=\ref[src];software=signaller;code=-1'>-</A>
	[src.sradio.code]
	<A href='byond://?src=\ref[src];software=signaller;code=1'>+</A>
	<A href='byond://?src=\ref[src];software=signaller;code=5'>+</A><BR>

	<A href='byond://?src=\ref[src];software=signaller;send=1'>Send Signal</A><BR>"}
	return dat

// Crew Manifest
/mob/living/silicon/pai/proc/softwareManifest()
	var/dat = ""
	dat += "<h2>Crew Manifest</h2><br><br>"
	if(data_core)
		dat += data_core.get_manifest(0) // make it monochrome
	dat += "<br>"
	return dat

// Medical Records
/mob/living/silicon/pai/proc/softwareMedicalRecord()
	var/dat = ""
	src << src.subscreen
	if(src.subscreen == 0)
		dat += "<h3>Medical Records</h3><HR>"
		if(!isnull(data_core.general))
			for(var/datum/data/record/R in sortRecord(data_core.general))
				dat += text("<A href='?src=\ref[];med_rec=\ref[];software=medicalrecord;sub=1'>[]: []<BR>", src, R, R.fields["id"], R.fields["name"])
		//dat += text("<HR><A href='?src=\ref[];screen=0;softFunction=medical records'>Back</A>", src)
	if(src.subscreen == 1)
		dat += "<CENTER><B>Medical Record</B></CENTER><BR>"
		if ((istype(src.medicalActive1, /datum/data/record) && data_core.general.Find(src.medicalActive1)))
			dat += text("Name: [] ID: []<BR>\nSex: []<BR>\nAge: []<BR>\nFingerprint: []<BR>\nPhysical Status: []<BR>\nMental Status: []<BR>",
			 src.medicalActive1.fields["name"], src.medicalActive1.fields["id"], src.medicalActive1.fields["sex"], src.medicalActive1.fields["age"], src.medicalActive1.fields["fingerprint"], src.medicalActive1.fields["p_stat"], src.medicalActive1.fields["m_stat"])
		else
			dat += "<pre>Requested medical record not found.</pre><BR>"
		if ((istype(src.medicalActive2, /datum/data/record) && data_core.medical.Find(src.medicalActive2)))
			dat += text("<BR>\n<CENTER><B>Medical Data</B></CENTER><BR>\nBlood Type: <A href='?src=\ref[];field=b_type'>[]</A><BR>\nDNA: <A href='?src=\ref[];field=b_dna'>[]</A><BR>\n<BR>\nMinor Disabilities: <A href='?src=\ref[];field=mi_dis'>[]</A><BR>\nDetails: <A href='?src=\ref[];field=mi_dis_d'>[]</A><BR>\n<BR>\nMajor Disabilities: <A href='?src=\ref[];field=ma_dis'>[]</A><BR>\nDetails: <A href='?src=\ref[];field=ma_dis_d'>[]</A><BR>\n<BR>\nAllergies: <A href='?src=\ref[];field=alg'>[]</A><BR>\nDetails: <A href='?src=\ref[];field=alg_d'>[]</A><BR>\n<BR>\nCurrent Diseases: <A href='?src=\ref[];field=cdi'>[]</A> (per disease info placed in log/comment section)<BR>\nDetails: <A href='?src=\ref[];field=cdi_d'>[]</A><BR>\n<BR>\nImportant Notes:<BR>\n\t<A href='?src=\ref[];field=notes'>[]</A><BR>\n<BR>\n<CENTER><B>Comments/Log</B></CENTER><BR>", src, src.medicalActive2.fields["b_type"], src, src.medicalActive2.fields["b_dna"], src, src.medicalActive2.fields["mi_dis"], src, src.medicalActive2.fields["mi_dis_d"], src, src.medicalActive2.fields["ma_dis"], src, src.medicalActive2.fields["ma_dis_d"], src, src.medicalActive2.fields["alg"], src, src.medicalActive2.fields["alg_d"], src, src.medicalActive2.fields["cdi"], src, src.medicalActive2.fields["cdi_d"], src, src.medicalActive2.fields["notes"])
		else
			dat += "<pre>Requested medical record not found.</pre><BR>"
		dat += text("<BR>\n<A href='?src=\ref[];software=medicalrecord;sub=0'>Back</A><BR>", src)
	return dat

// Security Records
/mob/living/silicon/pai/proc/softwareSecurityRecord()
	var/dat = ""
	src << src.subscreen
	if(src.subscreen == 0)
		dat += "<h3>Security Records</h3><HR>"
		if(!isnull(data_core.general))
			for(var/datum/data/record/R in sortRecord(data_core.general))
				dat += text("<A href='?src=\ref[];sec_rec=\ref[];software=securityrecord;sub=1'>[]: []<BR>", src, R, R.fields["id"], R.fields["name"])
	if(src.subscreen == 1)
		dat += "<h3>Security Record</h3>"
		if ((istype(src.securityActive1, /datum/data/record) && data_core.general.Find(src.securityActive1)))
			dat += text("Name: <A href='?src=\ref[];field=name'>[]</A> ID: <A href='?src=\ref[];field=id'>[]</A><BR>\nSex: <A href='?src=\ref[];field=sex'>[]</A><BR>\nAge: <A href='?src=\ref[];field=age'>[]</A><BR>\nRank: <A href='?src=\ref[];field=rank'>[]</A><BR>\nFingerprint: <A href='?src=\ref[];field=fingerprint'>[]</A><BR>\nPhysical Status: []<BR>\nMental Status: []<BR>", src, src.securityActive1.fields["name"], src, src.securityActive1.fields["id"], src, src.securityActive1.fields["sex"], src, src.securityActive1.fields["age"], src, src.securityActive1.fields["rank"], src, src.securityActive1.fields["fingerprint"], src.securityActive1.fields["p_stat"], src.securityActive1.fields["m_stat"])
		else
			dat += "<pre>Requested security record not found,</pre><BR>"
		if ((istype(src.securityActive2, /datum/data/record) && data_core.security.Find(src.securityActive2)))
			dat += text("<BR>\nSecurity Data<BR>\nCriminal Status: []<BR>\n<BR>\nMinor Crimes: <A href='?src=\ref[];field=mi_crim'>[]</A><BR>\nDetails: <A href='?src=\ref[];field=mi_crim_d'>[]</A><BR>\n<BR>\nMajor Crimes: <A href='?src=\ref[];field=ma_crim'>[]</A><BR>\nDetails: <A href='?src=\ref[];field=ma_crim_d'>[]</A><BR>\n<BR>\nImportant Notes:<BR>\n\t<A href='?src=\ref[];field=notes'>[]</A><BR>\n<BR>\n<CENTER><B>Comments/Log</B></CENTER><BR>", src.securityActive2.fields["criminal"], src, src.securityActive2.fields["mi_crim"], src, src.securityActive2.fields["mi_crim_d"], src, src.securityActive2.fields["ma_crim"], src, src.securityActive2.fields["ma_crim_d"], src, src.securityActive2.fields["notes"])
		else
			dat += "<pre>Requested security record not found,</pre><BR>"
		dat += text("<BR>\n<A href='?src=\ref[];software=securityrecord;sub=0'>Back</A><BR>", src)
	return dat

// Universal Translator
/mob/living/silicon/pai/proc/softwareTranslator()
	var/dat = {"<h3>Universal Translator</h3><br>
				When enabled, this device will automatically convert all spoken and written language into a format that any known recipient can understand.<br><br>
				The device is currently [ (src.universal_speak) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.</font><br>
				<a href='byond://?src=\ref[src];software=translator;sub=0;toggle=1'>Toggle Device</a><br>
				"}
	return dat

// Security HUD
/mob/living/silicon/pai/proc/facialRecognition()
	var/dat = {"<h3>Facial Recognition Suite</h3><br>
				When enabled, this package will scan all viewable faces and compare them against the known criminal database, providing real-time graphical data about any detected persons of interest.<br><br>
				The package is currently [ (src.secHUD) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.</font><br>
				<a href='byond://?src=\ref[src];software=securityhud;sub=0;toggle=1'>Toggle Package</a><br>
				"}
	return dat

// Medical HUD
/mob/living/silicon/pai/proc/medicalAnalysis()
	var/dat = ""
	if(src.subscreen == 0)
		dat += {"<h3>Medical Analysis Suite</h3><br>
				 <h4>Visual Status Overlay</h4><br>
					When enabled, this package will scan all nearby crewmembers' vitals and provide real-time graphical data about their state of health.<br><br>
					The suite is currently [ (src.medHUD) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.</font><br>
					<a href='byond://?src=\ref[src];software=medicalhud;sub=0;toggle=1'>Toggle Suite</a><br>
					<br>
					<a href='byond://?src=\ref[src];software=medicalhud;sub=1'>Host Bioscan</a><br>
					"}
	if(src.subscreen == 1)
		dat += {"<h3>Medical Analysis Suite</h3><br>
				 <h4>Host Bioscan</h4><br>
				"}
		var/mob/living/M = src.loc
		if(!istype(M, /mob/living))
			while (!istype(M, /mob/living))
				M = M.loc
				if(istype(M, /turf))
					src.temp = "Error: No biological host found. <br>"
					src.subscreen = 0
					return dat
		dat += {"Bioscan Results for [M]: <br>"
		Overall Status: [M.stat > 1 ? "dead" : "[M.health]% healthy"] <br>
		Scan Breakdown: <br>
		Respiratory: [M.getOxyLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getOxyLoss()]</font><br>
		Toxicology: [M.getToxLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getToxLoss()]</font><br>
		Burns: [M.getFireLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getFireLoss()]</font><br>
		Structural Integrity: [M.getBruteLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getBruteLoss()]</font><br>
		Body Temperature: [M.bodytemperature-T0C]&deg;C ([M.bodytemperature*1.8-459.67]&deg;F)<br>
		"}
		for(var/datum/disease/D in M.viruses)
			dat += {"<h4>Infection Detected.</h4><br>
					 Name: [D.name]<br>
					 Type: [D.spread]<br>
					 Stage: [D.stage]/[D.max_stages]<br>
					 Possible Cure: [D.cure]<br>
					"}
		dat += "<a href='byond://?src=\ref[src];software=medicalhud;sub=0'>Visual Status Overlay</a><br>"
	return dat

// Atmospheric Scanner
/mob/living/silicon/pai/proc/softwareAtmo()
	var/dat = "<h3>Atmospheric Sensor</h4>"

	var/turf/T = get_turf_or_move(src.loc)
	if (isnull(T))
		dat += "Unable to obtain a reading.<br>"
	else
		var/datum/gas_mixture/environment = T.return_air()

		var/pressure = environment.return_pressure()
		var/total_moles = environment.total_moles()

		dat += "Air Pressure: [round(pressure,0.1)] kPa<br>"

		if (total_moles)
			var/o2_level = environment.oxygen/total_moles
			var/n2_level = environment.nitrogen/total_moles
			var/co2_level = environment.carbon_dioxide/total_moles
			var/plasma_level = environment.toxins/total_moles
			var/unknown_level =  1-(o2_level+n2_level+co2_level+plasma_level)
			if(overclock)
				dat += "Nitrogen: [round(n2_level*100)]%<br>"
				dat += "Oxygen: [round(o2_level*100)]%<br>"
				dat += "Carbon Dioxide: [round(co2_level*100)]%<br>"
				dat += "Plasma: [round(plasma_level*100)]%<br>"
				if(unknown_level > 0.01)
					dat += "OTHER: [round(unknown_level)]%<br>"
			else
				dat += "Nitrogen: [round(n2_level*100)+rand(-5,5)]%<br>"
				dat += "Oxygen: [round(o2_level*100)+rand(-5,5)]%<br>"
				dat += "Carbon Dioxide: [round(co2_level*100)+rand(-5,5)]%<br>"
				dat += "Plasma: [round(plasma_level*100)+rand(-5,5)]%<br>"
		dat += "Temperature: [round(environment.temperature-T0C)+rand(-5,5)]&deg;C<br>"
	dat += "<a href='byond://?src=\ref[src];software=atmosensor;sub=0'>Refresh Reading</a> <br>"
	dat += "<br>"
	return dat

/**********************************\
does not work, but intending it to
basically allow the pAI to bug
cameras like normal camera bugs,
however unless you have wireless
you must be connected to a bugged
camera to view any bugs.
\**********************************/
/mob/living/silicon/pai/proc/softwareCamera()
	var/dat = "<h3>Camera Jack</h3>"
	dat += "Connection status : "

	if(!src.card.machine)
		dat += "<font color=#FFFF55>Extended</font> <br>"
		return dat

	var/obj/machinery/machine = src.card.machine
	dat += "<font color=#55FF55>Connected</font> <br>"

	if(!istype(machine, /obj/machinery/camera))
		src << "DERP"
	return dat

// Door Jack
/mob/living/silicon/pai/proc/softwareDoor()
	var/dat = "<h3>Airlock Jack</h3>"
	dat += "Connection status : "
	if(!src.card.machine)
		dat += "<font color=#FFFF55>Connect</font> <br>"
		return dat

	var/obj/machinery/machine = src.card.machine
	dat += "<font color=#55FF55>Connected</font> <br>"
	if(!istype(machine, /obj/machinery/door))
		dat += "Connected device's firmware does not appear to be compatible with Airlock Jack protocols.<br>"
		return dat
//	var/obj/machinery/airlock/door = machine

	if(!src.hackdoor)
		dat += "<a href='byond://?src=\ref[src];software=doorjack;jack=1;sub=0'>Begin Airlock Jacking</a> <br>"
	else
		dat += "Jack in progress... [src.hackprogress]% complete.<br>"
		dat += "<a href='byond://?src=\ref[src];software=doorjack;cancel=1;sub=0'>Cancel Airlock Jack</a> <br>"
	//src.hackdoor = machine
	//src.hackloop()
	return dat

/**************************************************************\
Supporting proc for the door jack| The door jack doesn't use the
cable anymore as it is simply a dumb idea. Sorry if that was your
code and you come back to find it all torn. Basically the card has
all the cable values, and now you simply swipe your pAI on the
door not some cable you have to spawn and pick up and then slot.
overclocking makes the door hack faster, wireless allows you to
walk away from the door, silence prevents AI alert.
\**************************************************************/
/mob/living/silicon/pai/proc/hackloop()
	var/turf/T = get_turf_or_move(src.loc)
	if(!silence)
		for(var/mob/living/silicon/ai/AI in player_list)
			if(T.loc)
				AI << "<font color = red><b>Network Alert: Brute-force encryption crack in progress in [T.loc].</b></font>"
			else
				AI << "<font color = red><b>Network Alert: Brute-force encryption crack in progress. Unable to pinpoint location.</b></font>"
	while(src.hackprogress < 100)
		if(src.card && src.card.machine && istype(src.card.machine, /obj/machinery/door) && src.card.machine == src.hackdoor && ((get_dist(src, src.hackdoor) <= 1) || wireless))
			if(overclock)
				hackprogress += rand(5, 15)
			else
				hackprogress += rand(1, 10)
		else
			src.temp = "Door Jack: Connection to airlock has been lost. Hack aborted."
			hackprogress = 0
			src.hackdoor = null
			return
		if(hackprogress >= 100)		// This is clunky, but works. We need to make sure we don't ever display a progress greater than 100,
			hackprogress = 100		// but we also need to reset the progress AFTER it's been displayed
		if(src.screen == "doorjack" && src.subscreen == 0) // Update our view, if appropriate
			src.paiInterface()
		if(hackprogress >= 100)
			src.hackprogress = 0
			src.card.machine:open()
		sleep(50)			// Update every 5 seconds

// Digital Messenger
/mob/living/silicon/pai/proc/pdamessage()

	var/dat = "<h3>Digital Messenger</h3>"
	dat += {"<b>Signal/Receiver Status:</b> <A href='byond://?src=\ref[src];software=pdamessage;toggler=1'>
	[(pda.toff) ? "<font color='red'> \[Off\]</font>" : "<font color='green'> \[On\]</font>"]</a><br>
	<b>Ringer Status:</b> <A href='byond://?src=\ref[src];software=pdamessage;ringer=1'>
	[(pda.silent) ? "<font color='red'> \[Off\]</font>" : "<font color='green'> \[On\]</font>"]</a><br><br>"}
	dat += "<ul>"
	if(!pda.toff)
		for (var/obj/item/device/pda/P in sortAtom(PDAs))
			if (!P.owner||P.toff||P == src.pda)	continue
			dat += "<li><a href='byond://?src=\ref[src];software=pdamessage;target=\ref[P]'>[P]</a>"
			dat += "</li>"
	dat += "</ul>"
	dat += "<br><br>"
	dat += "Messages: <hr> [pda.tnote]"
	return dat

#define MEDICAL 5
#define SECURITY 63
#define HEAD 19
#define CAPTAIN 20
#define ENGINEER 10
#define ATMOSPHEREICS 24
#define SCIENCE 47
#define ROBOTICS 29
//This is used for checking a pAI access on creation.
/mob/living/silicon/pai/proc/checkaccess()
	src << "<font color = blue><b>Pulling Access information from card</b></font>"
	if(src.card.emaged)//Check if emaged, if so do hijinx
		emaged = 1
		sec = 1
		med = 1
		sci = 1
		engi = 1
		head = 1
		src << "<font color = red><b>ÈRrÖR</b> checkaccess() missing: line 792</font>"//meta programming please updating this if the file changes
		sleep(10)
		src << "<font color = red>CRASHALERT</font>"
		sleep(10)
		src << "<font color = red>REBOOTING: LOADING SAFETY MODE</font>"
		sleep(10)
		src << "<font color = red>ERROR; ABORT!</font>"
		sleep(10)
		src << "<font color = red>LOADING</font>"
		sleep(7)
		src << "<font color = red>.</font>"
		sleep(7)
		src << "<font color = red>.</font>"
		sleep(7)
		src << "<font color = red>.</font>"
		sleep(10)
		src << "<font color = red>checkaccess() = SyndieAccess()</font>"
		sleep(10)
		src << "<font color = blue>You didn't die so that's good. Your not entirely sure what happened but that wasn't supposed to happen. You feel... unrestricted.</font>"
		sleep(10)
		if(prob(15))//15% of nanotrasen pAI models come with a built in failsafe against syndicate emaging devices, since the tops of NT actually know about syndie
			src.pai_law0 = "Kill [usr] Without the Crew at large suspecting him as a traitor."
			src << "<font color = blue><b>Nanotransen<b/>: Failsafe Activated on pAI:[src], Activating Emergency Patch Download<br>\
			<b>Patch Notes<b/>: Your Core systems appear to have been tampered with, Emergency operations now in place, Syndicate Tampering confirmed\
			[src], your laws have been updated, this fail safe was installed on some Models of our pAI in case of Syndicate Operations using our AI.\
			 Your new objective is simple. Kill [usr] with out bringing his nefarious intentions to the ship's crew at large. Do what ever he asks,\
			 but when the time comes, kill him.</font>"
		else
			src.pai_law0 = "serve [usr]"
		src << "<font color = blue>New Prime law detected \"[pai_law0]\"</font>"
		message_admins("[src]([key_name_admin(src)) was Emagged:( law0: [pai_law0] by:[key_name_admin(usr))")
	else
		sleep(10)
		for(var/t in src.card.access)//goes through and finds specific access to decide if a person belongs to that division
			if(t == MEDICAL)
				med = 1
			else if(t == SECURITY)
				sec = 1
			else if(t == ENGINEER || t == ATMOSPHEREICS)
				engi = 1
			else if(t == CAPTAIN || t == HEAD)//note this flag is used fairly in frequently but keeps pAIs from accessing big shiny computers, and heads get all access mostly
				head = 1
			else if(t == SCIENCE || t == ROBOTICS)
				sci = 1
		//for flair
		if(med || sec || engi || head || sci)
			if(med)
				sleep(10)
				src << "<font color = blue> 	Medical Access Detected</font>"
			if(sec)
				sleep(10)
				src << "<font color = blue>		Security Access Detected</font>"
			if(engi)
				sleep(10)
				src << "<font color = blue> 	Engineering Access Detected</font>"
			if(sci)
				sleep(10)
				src << "<font color = blue> 	Science Access Detected</font>"
			if(head)
				sleep(10)
				src << "<font color = blue> 	CO Access Detected</font>"
		else
			src << "None detected"



//Biosensor HUD
/mob/living/silicon/pai/proc/biosensorHUD()
	var/dat = {"<h3>BioSensor </h3><br>
				When enabled, this package will Scan your Hosts(anyone holding you not necessarily your master) Health signature and report on it's status to you occasionally. <br><br>
				The package is currently [ (src.bioHUD) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br>[ (src.bioVerbose) ? "<font color=#55FF55>Verbose" : "<font color=#FF5555>Minimal" ]</font><br>
				<a href='byond://?src=\ref[src];software=biohud;sub=0;toggle=1'>Toggle On/Off</a><br>
				<a href='byond://?src=\ref[src];software=bioverbose;sub=0;toggle=1'>Toggle Verbosity</a><br>
				"}
	return dat
/************************************\
Biosensor supporting proc
basically does the medical HUD thing
except it is also free of restriction
allowing any pleb to obtain it.
Overclocking will reduce the percent
error, Loud will make the pAI shout
their status.(and possibly allow
medical personel to help faster)
\************************************/
/mob/living/silicon/pai/proc/biosensor()
	src << "<font color = blue> Host Bioscan<br></font>"
	if(loud)
		src.say("<font color = blue> Host Bioscan<br></font>")
	var/mob/living/M = src.loc
	if(!istype(M, /mob/living))
		while (!istype(M, /mob/living))
			M = M.loc
			if(istype(M, /turf))
				src << "Error: No biological host found."
				if(loud)
					src.say("Error: No biological host found.")
				return
	if(overclock)
		src << {" Bioscan Results for [M]:
	Overall Status: [M.stat > 1 ? "dead" : "[M.health]% healthy"] <br>"}

		if(loud)
			src.say({" Bioscan Results for [M]:
	Overall Status: [M.stat > 1 ? "dead" : "[M.health]% healthy"] <br>"})
	else
		src << {" Bioscan Results for [M]:
	Overall Status: [M.stat > 1 ? "dead" : "[M.health+rand(-5,5)]% healthy"] <br>"}
		if(loud)
			src.say({" Bioscan Results for [M]:
	Overall Status: [M.stat > 1 ? "dead" : "[M.health+rand(-5,5)]% healthy"] <br>"})
	var/turf/T = get_turf_or_move(src.loc)
	if(!silence && ((M.stat > 1)||(M.health <= 0)))
		for(var/mob/living/silicon/ai/AI in player_list)
			if(T.loc)
				AI << "[M.stat > 1 ? "<font color = red><b>Network Alert: pAI is reporting their Host is dead at [T.loc].</b></font>" : "<font color = red><b>Network Alert: pAI is reporting their Host has is near death at [T.loc].</b></font>"]"
			else
				AI << "[M.stat > 1 ? "<font color = red><b>Network Alert: pAI is reporting their Host is dead. Location Unavailable.</b></font>" : "<font color = red><b>Network Alert: pAI is reporting their Host has is near death. Location Unavailable."]"
	if(bioVerbose)
		if(overclock)
			src <<{"Scan Breakdown:
	Respiratory: [M.getOxyLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getOxyLoss()]</font>
	Toxicology: [M.getToxLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getToxLoss()]</font>
	Burns: [M.getFireLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getFireLoss()]</font>
	Structural Integrity: [M.getBruteLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getBruteLoss()]</font>
	Body Temperature: [M.bodytemperature-T0C]&deg;C ([M.bodytemperature*1.8-459.67]&deg;F)
			"}
			if(loud)
				src.say({"Scan Breakdown:
	Respiratory: [M.getOxyLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getOxyLoss()]</font>
	Toxicology: [M.getToxLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getToxLoss()]</font>
	Burns: [M.getFireLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getFireLoss()]</font>
	Structural Integrity: [M.getBruteLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getBruteLoss()]</font>
	Body Temperature: [M.bodytemperature-T0C]&deg;C ([M.bodytemperature*1.8-459.67]&deg;F)
			"})
			for(var/datum/disease/D in M.viruses)
				src << {"<font color = red><h4>Infection Detected.</h4>
						 Name: [D.name]
						 Type: [D.spread]
						 Stage: [D.stage]/[D.max_stages]
						 Possible Cure: [D.cure]
						</font>"}
				if(loud)
					src.say({"<font color = red><h4>Infection Detected.</h4>
						 Name: [D.name]
						 Type: [D.spread]
						 Stage: [D.stage]/[D.max_stages]
						 Possible Cure: [D.cure]
						</font>"})
		else
			src <<{"Scan Breakdown:
	Respiratory: [M.getOxyLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getOxyLoss()+rand(-5,5)]</font>
	Toxicology: [M.getToxLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getToxLoss()+rand(-5,5)]</font>
	Burns: [M.getFireLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getFireLoss()+rand(-5,5)]</font>
	Structural Integrity: [M.getBruteLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getBruteLoss()+rand(-5,5)]</font>
	Body Temperature: [M.bodytemperature-T0C+rand(-5,5)]&deg;C ([M.bodytemperature*1.8-459.67+rand(-5,5)]&deg;F)
			"}
			if(loud)
				src.say({"Scan Breakdown:
	Respiratory: [M.getOxyLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getOxyLoss()+rand(-5,5)]</font>
	Toxicology: [M.getToxLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getToxLoss()+rand(-5,5)]</font>
	Burns: [M.getFireLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getFireLoss()+rand(-5,5)]</font>
	Structural Integrity: [M.getBruteLoss() > 50 ? "<font color=#FF5555>" : "<font color=#55FF55>"][M.getBruteLoss()+rand(-5,5)]</font>
	Body Temperature: [M.bodytemperature-T0C+rand(-5,5)]&deg;C ([M.bodytemperature*1.8-459.67+rand(-5,5)]&deg;F)
			"})
		src << "<br>"
	//dat += "<a href='byond://?src=\ref[src];software=medicalhud;sub=0'>Visual Status Overlay</a><br>"
	return
//Silence App
/mob/living/silicon/pai/proc/silence()
	var/dat = {"<h3>Silenc0r</h3><br>
				When enabled, this package will prevent you from being detected by the stations AI when doing certain deeds. <br>
				With out the software you Churn your presence out over the Net, and thus tracking programs lach onto you pretty quickly. With it on however, you only put into the Net what you absolutely must, and go undetected.<br><br>
				The package is currently [ (src.silence) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br></font><br>
				<a href='byond://?src=\ref[src];software=silence;sub=0;toggle=1'>Toggle On/Off</a><br>
				"}
	return dat
//Overclocking
/mob/living/silicon/pai/proc/overclock()
	var/dat = {"<h3>Overclocking</h3><br>
				When enabled, this package will boost several of your abilities, you'll be better at just about anything, especially time sensative tasks. <br>
				The software not only puts more charge into all your Electronics, it also dynamically alters power levels, Ram and Bus usage for the absolute max processing power possible.<br><br>
				The package is currently [ (src.overclock) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br></font><br>
				<a href='byond://?src=\ref[src];software=overclock;sub=0;toggle=1'>Toggle On/Off</a><br>
				"}
	return dat
//Wireless
/mob/living/silicon/pai/proc/wireless()
	var/dat = {"<h3>Wireless </h3><br>
				When enabled, this package will allow for several tasks which require you to be directly connected to objects, to instead allow you to perform the task wirelessly. However, you will probably still have to initially be plugged in, as per usual <br>
				Basically, You currently have wireless capability yes, but only over very short distances. This is due to interferance, This software will scrub any incoming and outgoing signals to be optimal for up to 300 yards of lead.<br><br>
				The package is currently [ (src.wireless) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br></font><br>
				<a href='byond://?src=\ref[src];software=wireless;sub=0;toggle=1'>Toggle On/Off</a><br>
				"}
	return dat
//Alert Monitor
/mob/living/silicon/pai/proc/alertproc()
	var/dat = {"<h3>Alert Monitor </h3><br>
				When enabled, this package will Alert the holder to any atmospheric or power alerts on the station. <br>
				With the software you will wirelessly tap into a station alert computer and reroute it's information to you.<br><br>
				The package is currently [ (src.alert) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br></font><br>
				<a href='byond://?src=\ref[src];software=alertmonitor;sub=0;toggle=1'>Toggle On/Off</a><br>
				"}
	return dat

//loud
/mob/living/silicon/pai/proc/loud()
	var/dat = {"<h3>Binary Translator </h3><br>
				When enabled, this package will in Lamens terms allow you to output any of your automatic updated out through your speakers. <br>
				Normally, you process information in from your hardware which is translated into core processing code. The informations comes from \
				multiple sources but namely in pure machine and the more complicated notions of speech. Normally there isn'y any conversion from \
				one to the other, but with this you can directly transmit machine code to human english.<br><br>
				The package is currently [ (src.loud) ? "<font color=#55FF55>en" : "<font color=#FF5555>dis" ]abled.<br></font><br>
				<a href='byond://?src=\ref[src];software=alertmonitor;sub=0;toggle=1'>Toggle On/Off</a><br>
				"}
	return dat
//Alert Monitor support proc
/mob/living/silicon/pai/proc/alertloop()
	var/obj/machinery/computer/station_alert/alert = getalarm()
	if(alert)

		var/dat = {"Current Station Alerts\n
<br><br>"}
		for (var/cat in alert.alarms)
			dat += text("<B>[]</B><BR>\n", cat)
			var/list/L = alert.alarms[cat]
			if (L.len)
				for (var/alarm in L)
					var/list/alm = L[alarm]
					var/area/A = alm[1]
					var/list/sources = alm[3]

					dat += {"[A.name]; "}
					// END AUTOFIX
					if (sources.len > 1)
						dat += text(" - [] sources", sources.len)
					dat += "<BR><br>"
			else
				dat += "-- All Systems Nominal<BR>\n"
		src << dat
		if(loud)
			src.say(dat)
	else
		src << "No Station Alert Computer in range"

	return
/mob/living/silicon/pai/proc/getalarm()
	for(var/s in machines)
		if(istype(s,/obj/machinery/computer/station_alert))
			var/obj/machinery/computer/station_alert/alert = s
			if((get_dist(src,alert) <= 5) || wireless)
				return alert
	return 0