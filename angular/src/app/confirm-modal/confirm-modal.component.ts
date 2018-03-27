import { Component, OnInit, ViewChild, Input } from '@angular/core';
import { NgbModal, ModalDismissReasons } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'app-confirm-modal',
  templateUrl: './confirm-modal.component.html',
  styleUrls: ['./confirm-modal.component.scss']
})
export class ConfirmModalComponent implements OnInit {
  closeResult: string;

  @ViewChild('content') private content: any;

  // Confirmation message
  @Input() confirmationMessage: string;

  // Modal message body
  public modalBody: string;

  constructor(private modalService: NgbModal) { }

  public ngOnInit(): void { }

  public open(message: string): Promise<boolean> {
    this.modalBody = message;

    const promise = new Promise<boolean>((resolve, reject) => {
      this.modalService.open(this.content).result.then(
        (result) => {
          this.closeResult = `Closed with: ${result}`;
          resolve(true);
        }, (reason) => {
          this.closeResult = `Dismissed ${this.getDismissReason(reason)}`;
          reject(false);
        });
    });

    return promise;
  }

  public getDismissReason(reason: any): string {
    if (reason === ModalDismissReasons.ESC) {
      return 'by pressing ESC';
    } else if (reason === ModalDismissReasons.BACKDROP_CLICK) {
      return 'by clicking on a backdrop';
    } else {
      return `with: ${reason}`;
    }
  }
}
