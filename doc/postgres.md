# Guide on setting up Postgres to use with py-mgr

This repository is bundled with `py-mgr` data lab, which is a python application layered on top of Postgres. It serves as a data manager for labelled data. I created it to store my user agent strings together with the labels that I have assigned to them.

## Install Postgres

Tested with version 9.2.

`yum install postgres postgres-cli`

## Initial setup

## User, database, and tables

`py-mgr` by default uses a user called `client` with password `client` and a user `uadet`.
