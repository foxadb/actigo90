import { Component } from '@angular/core';
import { Router } from '@angular/router';

import { AuthenticationService } from './services/authentication.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {

  constructor(
    private router: Router,
    private auth: AuthenticationService
  ) { }

  public onLogout(): void {
    this.auth.logout();
    this.router.navigate(['']);
  }

  public isAuthenticated(): boolean {
    return this.auth.isAuthenticated();
  }

  public isUser(): boolean {
    return this.auth.getRole() === 'user';
  }

  public isAdmin(): boolean {
    return this.auth.getRole() === 'admin';
  }

}
