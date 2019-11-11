
cp run-project.service /lib/systemd/system

sudo systemctl daemon-reload

sudo systemctl enable run-project

sudo systemctl status run-project

sudo systemctl disable run-project

sudo systemctl start run-project

sudo systemctl stop run-project


