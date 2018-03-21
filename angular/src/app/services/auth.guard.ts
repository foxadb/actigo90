import { Injectable } from '@angular/core';
import { Router, CanActivate, ActivatedRouteSnapshot } from '@angular/router';
import { JwtHelperService } from '@auth0/angular-jwt';
import { AuthenticationService } from '../services/authentication.service';

@Injectable()
export class AuthGuard implements CanActivate {

  private jwtHelper = new JwtHelperService();

  constructor(
    private auth: AuthenticationService,
    private router: Router) { }

  public canActivate(route: ActivatedRouteSnapshot): boolean {
    // expected role to access to route
    const expectedRole = route.data.expectedRole;

    // retrieve the current user
    const currentUser = JSON.parse(localStorage.getItem('currentUser'));

    if (currentUser) {
      // decode the token payload
      const tokenPayload = this.jwtHelper.decodeToken(currentUser.token);

      if (this.auth.isAuthenticated() && this.checkPermission(tokenPayload, expectedRole)) {
        return true;
      }
    }

    // if no authenticate, redirect to login page
    this.router.navigate(['login']);
    return false;
  }

  public checkPermission(tokenPayload: any, expectedRole: any): boolean {
    const role = tokenPayload.role;
    // allow admin to access the entire app
    return ((role === 'admin') || role === expectedRole);
  }

}
