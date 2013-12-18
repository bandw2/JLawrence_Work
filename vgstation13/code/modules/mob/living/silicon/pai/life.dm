/mob/living/silicon/pai/Life()
	if (src.stat == 2)
		return
	if(src.cable)
		if(get_dist(src, src.cable) > 1)
			var/turf/T = get_turf_or_move(src.loc)
			for (var/mob/M in viewers(T))
				M.show_message("\red [src.cable] rapidly retracts back into its spool.", 3, "\red You hear a click and the sound of wire spooling rapidly.", 2)
			del(src.cable)

	regular_hud_updates()
	if(src.secHUD == 1)
		src.securityHUD()
	if(src.medHUD == 1)
		src.medicalHUD()
	if(src.bioHUD == 1)
		if(world.timeofday >= bio_time)
			src.bio_time = world.timeofday + 20 * 10
			src.biosensor()
	if(src.alert == 1)
		if(world.timeofday >= alert_time)
			src.alert_time = world.timeofday + 20 * 10
			src.alertloop()
	if((world.timeofday >= silence_time) && silence_time != 0)
		silence_time = 0
		src << "<font color=green>Communication circuit reinitialized. Speech and messaging functionality restored.</font>"

	if((ram > max_ram +(overclock*50)) && silence_time == 0)//OVERLOAD
		src << "\red You Crashed."
		src.emp_act(ram-(max_ram +(overclock*50))/5)
		src.software = list()
		src.ram = src.ram/rand("1d20")
		src.secHUD = 0
		src.medHUD = 0
		src.bioHUD = 0
		src.bioVerbose = 0
		src.overclock = 0
		src.silence = 0
		src.wireless = 0
		src.alert = 0

/mob/living/silicon/pai/updatehealth()
	if(status_flags & GODMODE)
		health = 100
		stat = CONSCIOUS
	else
		health = 100 - getBruteLoss() - getFireLoss()

