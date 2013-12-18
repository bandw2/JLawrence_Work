/datum/job/ai
	title = "AI"
	flag = AI
	department_flag = ENGSEC
	faction = "Station"
	total_positions = 0
	spawn_positions = 1
	selection_color = "#ccffcc"
	supervisors = "your laws"
	req_admin_notify = 2
	minimal_player_age = 30

	equip(var/mob/living/carbon/human/H)
		if(!H)	return 0
		return 1


/datum/job/pai
	title = "pAI"
	flag = PAI
	department_flag = ENGSEC
	faction = "Station"
	total_positions = 0
	spawn_positions = 5
	selection_color = "#ddffdd"
	supervisors = "your laws and master"

	equip(var/mob/living/carbon/human/H)
		if(!H)	return 0
		return 1


/datum/job/cyborg
	title = "Cyborg"
	flag = CYBORG
	department_flag = ENGSEC
	faction = "Station"
	total_positions = 0
	spawn_positions = 2
	supervisors = "your laws and the AI"	//Nodrak
	selection_color = "#ddffdd"

	equip(var/mob/living/carbon/human/H)
		if(!H)	return 0
		return 1

/datum/job/mommi
	title = "Mobile MMI"
	flag = MOMMI
	department_flag = ENGSEC
	faction = "Station"
	total_positions = 0
	spawn_positions = 2
	supervisors = "your laws and the AI"	//Nodrak
	selection_color = "#ddffdd"

	equip(var/mob/living/carbon/human/H)
		if(!H)	return 0
		return 1