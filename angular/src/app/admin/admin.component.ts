import User from '../models/user.model';

import { Component, OnInit, ViewChild } from '@angular/core';
import { ConfirmModalComponent } from '../confirm-modal/confirm-modal.component';

import { UserService } from '../services/user.service';

@Component({
  selector: 'app-admin',
  templateUrl: './admin.component.html',
  styleUrls: ['./admin.component.scss']
})
export class AdminComponent implements OnInit {

  public users: Array<User>;

  public input: any = {};

  @ViewChild(ConfirmModalComponent) confirmModal: ConfirmModalComponent;

  public successMessageBox: string;
  public errorMessageBox: string;

  constructor(private userService: UserService) { }

  public ngOnInit(): void {
    this.getUsers();
  }

  public getUsers(): void {
    this.userService.getUsers().subscribe(
      users => {
        this.users = users;
      }
    );
  }

  // Collect user input to create the user body parameter for creation
  public newUser(): any {
    const username = this.input.username;
    const password = this.input.password;
    const role = this.input.role;

    if (username && password && role) {
      const user = {
        username: username,
        password: password,
        role: role
      };

      return user;
    } else {
      return undefined;
    }
  }

  // Create the user
  public createUser(): void {
    // Reset the message box
    this.successMessageBox = '';
    this.errorMessageBox = '';

    // Create the new user
    const newUser = this.newUser();

    if (newUser) {
      this.userService.createUser(newUser)
        .subscribe(
        res => {
          this.successMessageBox = 'User created!';

          // refresh the user list
          this.getUsers();
        },
        err => {
          this.errorMessageBox = 'Error when creating user';
        }
        );
    } else {
      this.errorMessageBox = 'Wrong user parameters';
    }
  }

  // Delete the user
  public deleteUser(id: string): void {
    // Reset the message box
    this.successMessageBox = '';
    this.errorMessageBox = '';

    this.confirmModal.open('Confirm you want to delete this user').then(
      () => {
        this.userService.deleteUser(id).subscribe(
          res => {
            this.successMessageBox = 'User deleted';

            // update the user list
            this.getUsers();
          },
          err => {
            this.errorMessageBox = 'Error when deleting user';
          }
        );
      },
      () => this.errorMessageBox = 'User not deleted'
    );
  }

}
