import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

import { AuthenticationService } from '../services/authentication.service';

@Component({
  selector: 'app-login',
  templateUrl: 'login.component.html',
  styleUrls: ['./login.component.scss']
})
export class LoginComponent implements OnInit {

  public model: any = {};
  public loading = false;
  public error = '';

  constructor(private auth: AuthenticationService, private router: Router) { }

  public ngOnInit(): void {
    // reset login status
    this.auth.logout();
  }

  public login(): void {
    this.loading = true;
    this.auth.login(this.model.username, this.model.password)
      .subscribe(
      result => {
        // login successful
        if (result === true) {
          switch (this.auth.getRole()) {
            case 'user': {
              this.router.navigate(['pricing']);
              break;
            }
            case 'admin': {
              this.router.navigate(['pricing']);
              break;
            }
          }
        }
      },
      error => {
        // login failed
        if (error === 'Unauthorized') {
          this.error = 'Wrong username or password';
          this.loading = false;
        }
      }
      );
  }

}
