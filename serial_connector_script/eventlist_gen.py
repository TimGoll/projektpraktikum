import random, os.path

### EVENTPARAMETER

amount_mfc = 1
amount_valve = 0
runtime = 60000 #ms
amount_events = 10

### EVENTPARAMETER ENDE

last_valve_value = []
eventlist = []

try:
    for i in range (0, amount_valve):
        last_valve_value.append(0)

    random_event_times = [ int(random.uniform(0, runtime)) for r in xrange(amount_events) ]
    random_event_times.sort()

    for i in range(0, len(random_event_times)-1):
        ev_id = random.randint(0, amount_mfc + amount_valve -1)
        if (ev_id < amount_mfc):
            eventlist.append('M,' + str(ev_id) + ',' + str(int(random.uniform(0.0,100.0))) + ',' + str(random_event_times[i]))
        else:
            ev_id -= amount_mfc
            last_valve_value[ev_id] = int(not last_valve_value[ev_id])
            eventlist.append('V,' + str(ev_id) + ',' + str(last_valve_value[ev_id]) + ',' + str(random_event_times[i]))

    #filename
    name_id = 0
    while (True):
        name = "programs/PROG%04d.TXT" % name_id
        if (os.path.isfile(name)):
            name_id+=1
        else:
            break

    file = open(name, 'w')

    file.write(str(amount_mfc) + ',' + str(amount_valve))
    file.write('\n')
    for i in range(0, amount_mfc):
        file.write("%02d" % i)
        if (i != amount_mfc -1):
            file.write(',')
    file.write('\n')
    for i in range(0, amount_mfc):
        file.write("MKS")
        if (i != amount_mfc -1):
            file.write(',')
    file.write('\n')
    file.write('0x20')
    file.write('\n')
    for i in range(0, amount_valve):
        file.write("0 " + str(i))
        if (i != amount_valve -1):
            file.write(',')
    file.write('\n')
    file.write('100')
    file.write('\n')
    file.write('00.00.0000-00:00:00')
    file.write('\n')
    file.write('\n')
    file.write('begin')
    file.write('\n')
    file.write('\n')


    for line in eventlist:
        file.write(line)
        file.write('\n')

    file.write('\n')
    file.write('end')
    file.write('\n')
    file.write('start')

    file.close()


except KeyboardInterrupt:
    end_program()
