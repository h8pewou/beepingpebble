module keyboard

fn C.getchar() u8

pub struct Config {
pub mut:
	keyboard_fn	fn (KeyCode) = unsafe { nil }
}

pub struct Manager {
pub mut:
	keyboard_fn	fn (KeyCode) = unsafe { nil }
}

fn (mut manager Manager) controller () {
	for ;; {
		key := wait_key()
		if key == 27 {
			unsafe {
				wait_key()
				x_key := {65:KeyCode(265),66:.down,67:.right,68:.left}[wait_key()]
				manager.keyboard_fn(x_key)
			}
		} else {
			unsafe {
				manager.keyboard_fn(KeyCode(key))
			}
		}
	}
}

pub fn new_manager (args Config) &Manager {
	mut manager := &Manager{
		keyboard_fn: args.keyboard_fn
	}
	go manager.controller()
	return manager
}

// pub enum KeyCode {
// 	right	= 262
// 	left	= 263
// 	down	= 264
// 	up		= 265
// 	a		= 97
// 	b
// 	c
// 	d
// 	e
// 	f
// 	g
// 	h
// 	i
// 	j
// 	k
// 	l
// 	m
// 	n
// 	o
// 	p
// 	q
// 	r
// 	s
// 	t
// 	u
// 	v
// 	w
// 	y
// 	z
// 	enter	= 13
// 	backspace = 127
// 	space   = 32
// 	escape	= 27
// 	tab		= 9
// 	menu	= -99
// 	delete
// }

pub enum KeyCode {
	invalid = 0
	space = 32
	apostrophe = 39 //'
	comma = 44 //,
	minus = 45 //-
	period = 46 //.
	slash = 47 ///
	_0 = 48
	_1 = 49
	_2 = 50
	_3 = 51
	_4 = 52
	_5 = 53
	_6 = 54
	_7 = 55
	_8 = 56
	_9 = 57
	semicolon = 59 //;
	equal = 61 //=
	a = 65
	b = 66
	c = 67
	d = 68
	e = 69
	f = 70
	g = 71
	h = 72
	i = 73
	j = 74
	k = 75
	l = 76
	m = 77
	n = 78
	o = 79
	p = 80
	q = 81
	r = 82
	s = 83
	t = 84
	u = 85
	v = 86
	w = 87
	x = 88
	y = 89
	z = 90
	left_bracket = 91 //[
	backslash = 92 //\
	right_bracket = 93 //]
	grave_accent = 96 //`
	world_1 = 161 // non-us #1
	world_2 = 162 // non-us #2
	escape = 256
	enter = 257
	tab = 258
	backspace = 259
	insert = 260
	delete = 261
	right = 262
	left = 263
	down = 264
	up = 265
	page_up = 266
	page_down = 267
	home = 268
	end = 269
	caps_lock = 280
	scroll_lock = 281
	num_lock = 282
	print_screen = 283
	pause = 284
	f1 = 290
	f2 = 291
	f3 = 292
	f4 = 293
	f5 = 294
	f6 = 295
	f7 = 296
	f8 = 297
	f9 = 298
	f10 = 299
	f11 = 300
	f12 = 301
	f13 = 302
	f14 = 303
	f15 = 304
	f16 = 305
	f17 = 306
	f18 = 307
	f19 = 308
	f20 = 309
	f21 = 310
	f22 = 311
	f23 = 312
	f24 = 313
	f25 = 314
	kp_0 = 320
	kp_1 = 321
	kp_2 = 322
	kp_3 = 323
	kp_4 = 324
	kp_5 = 325
	kp_6 = 326
	kp_7 = 327
	kp_8 = 328
	kp_9 = 329
	kp_decimal = 330
	kp_divide = 331
	kp_multiply = 332
	kp_subtract = 333
	kp_add = 334
	kp_enter = 335
	kp_equal = 336
	left_shift = 340
	left_control = 341
	left_alt = 342
	left_super = 343
	right_shift = 344
	right_control = 345
	right_alt = 346
	right_super = 347
	menu = 348
}

fn wait_key() u8 {
	keyboard.optimise_term()
	output := C.getchar()
	if output == 3 {
		keyboard.restore_term()
		exit(0)
	}
	return output
}
